// CLight.cpp

#include "CLight.h"
#include <glm/gtc/type_ptr.hpp>
#include "typedefs.h"

CLight::CLight(
    glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
    float constant, float linear, float quadratic) {
    _position = position; 
    _ambient = ambient;  _diffuse = diffuse;  _specular = specular;
    _ambient.w = 1.0f; _diffuse.w = 1.0f; _specular.w = 1.0f;
    _constant = constant; _linear = linear; _quadratic = quadratic;
    _intensity = 1.0f;
    _type = LightType::POINT; _posStart = position;
    _direction = glm::vec3(1.0f); _target = glm::vec3(1.0f);
    _innerCutOff = 0.0f; _outerCutOff = 0.0f; _exponent = 1.0f;
    // 代表 light 的模型，可自行更換
    _displayOn = true; _motionOn = false; _clock = 0.0f;
    _lighingOn = true; _lightObj.setPos(position);
}
// 方向式聚光燈建構元
//CLight::CLight(glm::vec3 position, glm::vec3 direction, float innerCutOffDeg, float outerCutOffDeg,
//    glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) {
//    _position = position; _direction = direction; _enabled = true;
//    _innerCutOff = innerCutOffDeg; _outerCutOff = outerCutOffDeg;
//    _ambient = ambient;   _diffuse = diffuse; _specular = specular;
//    _constant = constant; _linear = linear;   _quadratic = quadratic;
//}
// 目標式聚光燈建構元
CLight::CLight(glm::vec3 position, glm::vec3 target, float innerCutOffDeg, float outerCutOffDeg, float exponent,
    glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float constant, float linear, float quadratic) {
    _position = position; _target = target; _direction = glm::normalize(target - position);
    _intensity = 1.0f;  _exponent = exponent;
    _innerCutOff = glm::cos(glm::radians(innerCutOffDeg)); 
    _outerCutOff = glm::cos(glm::radians(outerCutOffDeg));
    _ambient = ambient;  _diffuse = diffuse;  _specular = specular;
    _ambient.w = 1.0f; _diffuse.w = 1.0f; _specular.w = 1.0f;
    _constant = constant; _linear = linear; _quadratic = quadratic;
    _type = LightType::SPOT; _posStart = position;
    _displayOn = true; _motionOn = false; _clock = 0.0f;
    _lighingOn = true; _lightObj.setPos(position);
}

CLight::~CLight() = default; // 讓編譯器產生預設的解構元

// 初始位置由建構元來設定，更新光源位置則是呼叫 setPos
void CLight::setPos(glm::vec3 pos) { 
    _position = pos; 
    // 如果是 spot light 則更新 direction
    if ( _type == LightType::SPOT ) {
        _direction = glm::normalize(_target - _position);
    }
}
glm::vec3 CLight::getPos()  { return _position; }

void CLight::setAmbient( glm::vec4 amb) { _ambient = amb; }
glm::vec4 CLight::getAmbient()  { return _ambient; }

void CLight::setDiffuse( glm::vec4 diff) { _diffuse = diff; }
glm::vec4 CLight::getDiffuse()  { return _diffuse; }

void CLight::setSpecular( glm::vec4 spec) { _specular = spec; }
glm::vec4 CLight::getSpecular()  { return _specular; }

void CLight::setIntensity(float intensity) { 
    _intensity = intensity;
    _ambient  = _ambient  * _intensity;
    _diffuse  = _diffuse  * _intensity;
    _specular = _specular * _intensity;
    _ambient.w = 1.0f; _diffuse.w = 1.0f; _specular.w = 1.0f;
}

void CLight::setAttenuation(float c, float l, float q) {
    _constant = c; _linear = l;  _quadratic = q;
}
void CLight::getAttenuation(float& c, float& l, float& q) {
    c = _constant; l = _linear;  q = _quadratic; 
}

void CLight::setLightOn(bool enable) { 
    _lighingOn = enable; 
    updateToShader();
}
bool CLight::isLightOn(){ return _lighingOn; }

void CLight::setMotionEnabled(bool enable) { _motionOn = enable; }

// 設定 shaderID 時，預設光源物件顯示為 true
void CLight::setShaderID(GLuint shaderProg, std::string name, bool displayon)
{
    _shaderID = shaderProg; _lightname = name;
    // 如果沒有啟用，就把強度設為零
    glm::vec4 amb = _lighingOn ? _ambient : glm::vec4(0.0f);
    glm::vec4 diff = _lighingOn ? _diffuse : glm::vec4(0.0f);
    glm::vec4 spec = _lighingOn ? _specular : glm::vec4(0.0f);

    GLint loc = glGetUniformLocation(_shaderID, (_lightname + ".position").c_str());
    glUniform3fv(loc, 1, glm::value_ptr(_position));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".ambient").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(amb));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".diffuse").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(diff));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".specular").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(spec));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".constant").c_str());
    glUniform1f(loc, _constant);

    loc = glGetUniformLocation(_shaderID, (_lightname + ".linear").c_str());
    glUniform1f(loc, _linear);

    loc = glGetUniformLocation(_shaderID, (_lightname + ".quadratic").c_str());
    glUniform1f(loc, _quadratic);

    loc = glGetUniformLocation(_shaderID, "lightType");
    glUniform1i(loc, _type);

    if ( _type == LightType::SPOT ) {
        loc = glGetUniformLocation(_shaderID, (_lightname + ".direction").c_str());
        glUniform3fv(loc, 1, glm::value_ptr(_direction));

        loc = glGetUniformLocation(_shaderID, (_lightname + ".cutOff").c_str());
        glUniform1f(loc, _innerCutOff);

        loc = glGetUniformLocation(_shaderID, (_lightname + ".outerCutOff").c_str());
        glUniform1f(loc, _outerCutOff);
    }
    _displayOn = displayon;
    if ( _displayOn ) { // 設定代表光源物件的顯示
        _lightObj.setupVertexAttributes();
        _lightObj.setShaderID(_shaderID);
        _lightObj.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
        _lightObj.setPos(_position);
    }
}

void CLight::updateToShader()
{
    //if (!_needsUpdate) return;
    
    // 如果沒有啟用，就把強度設為零
    glm::vec4 amb = _lighingOn ? _ambient : glm::vec4(0.0f);
    glm::vec4 diff = _lighingOn ? _diffuse : glm::vec4(0.0f);
    glm::vec4 spec = _lighingOn ? _specular : glm::vec4(0.0f);

    GLint loc = glGetUniformLocation(_shaderID, (_lightname + ".position").c_str());
    glUniform3fv(loc, 1, glm::value_ptr(_position));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".ambient").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(amb));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".diffuse").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(diff));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".specular").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(spec));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".constant").c_str());
    glUniform1f(loc, _constant);

    loc = glGetUniformLocation(_shaderID, (_lightname + ".linear").c_str());
    glUniform1f(loc, _linear);

    loc = glGetUniformLocation(_shaderID, (_lightname + ".quadratic").c_str());
    glUniform1f(loc, _quadratic);

    loc = glGetUniformLocation(_shaderID, "lightType");
    glUniform1i(loc, _type);

    if (_type == LightType::SPOT) {
        loc = glGetUniformLocation(_shaderID, (_lightname + ".direction").c_str());
        glUniform3fv(loc, 1, glm::value_ptr(_direction));

        loc = glGetUniformLocation(_shaderID, (_lightname + ".cutOff").c_str());
        glUniform1f(loc, _innerCutOff);

        loc = glGetUniformLocation(_shaderID, (_lightname + ".outerCutOff").c_str());
        glUniform1f(loc, _outerCutOff);

        loc = glGetUniformLocation(_shaderID, (_lightname + ".exponent").c_str());
        glUniform1f(loc, _exponent);
    }

    // _needsUpdate = false; // 每次更新後重設為 false
}

glm::vec3 CLight::getTarget() { return _target; }
glm::vec3 CLight::getDirection() { return _direction; }

void CLight::setTarget(const glm::vec3& target) {
    _target = target;
    _direction = glm::normalize(_target - _position);  // 計算 spot light 的照明方向
    _type = LightType::SPOT; // 設定成 spot light
}

void CLight::setCutOffDeg(float innerDeg, float outerDeg, float exponent) {
    _innerCutOff = glm::cos(glm::radians(innerDeg));
    _outerCutOff = glm::cos(glm::radians(outerDeg));
    _exponent = exponent;
    _type = LightType::SPOT;
}

void CLight::update(float dt)
{
    if (_motionOn) updateMotion(dt);
}

// 光源運動的計算 
void CLight::updateMotion(float dt) 
{ 
    // 目前實作以初始位置，繞著 Y 軸旋轉，每秒轉 180 度 
    glm::mat4 mxRot; 
    glm::vec4 pos; 
    _clock += dt; 
    if (_clock >= 4.0f) _clock = 0.0f;
    // 產生Y 軸旋轉矩陣, 並計算新的光源位置
    mxRot = glm::rotate(glm::mat4(1.0f), _clock * M_PI_2, glm::vec3(0.0f, 1.0f, 0.0f));
    pos = glm::vec4(_posStart, 1.0f);
    _position = glm::vec3(mxRot * pos);
    setPos(_position);
    if (_displayOn) _lightObj.setPos(_position);
    // 如果是 spot light，當光源有變動位置時，必須重新計算 direction
}

void CLight::draw()
{
    if( _displayOn ) _lightObj.draw(); // 顯示代表光源的模型
}

void CLight::drawRaw()
{
    if (_displayOn) _lightObj.drawRaw(); // 顯示代表光源的模型
}