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
    // �N�� light ���ҫ��A�i�ۦ��
    _displayOn = true; _motionOn = false; _clock = 0.0f;
    _lighingOn = true; _lightObj.setPos(position);
}
// ��V���E���O�غc��
//CLight::CLight(glm::vec3 position, glm::vec3 direction, float innerCutOffDeg, float outerCutOffDeg,
//    glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) {
//    _position = position; _direction = direction; _enabled = true;
//    _innerCutOff = innerCutOffDeg; _outerCutOff = outerCutOffDeg;
//    _ambient = ambient;   _diffuse = diffuse; _specular = specular;
//    _constant = constant; _linear = linear;   _quadratic = quadratic;
//}
// �ؼЦ��E���O�غc��
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

CLight::~CLight() = default; // ���sĶ�����͹w�]���Ѻc��

// ��l��m�ѫغc���ӳ]�w�A��s������m�h�O�I�s setPos
void CLight::setPos(glm::vec3 pos) { 
    _position = pos; 
    // �p�G�O spot light �h��s direction
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

void CLight::setLightOn(bool enable) { _lighingOn = enable; }
bool CLight::isLightOn(){ return _lighingOn; }

void CLight::setMotionEnabled() { _motionOn = !_motionOn; }

// �]�w shaderID �ɡA�w�]����������ܬ� true
void CLight::setShaderID(GLuint shaderProg, std::string name, bool displayon)
{
    _shaderID = shaderProg; _lightname = name;
    // �p�G�S���ҥΡA�N��j�׳]���s
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
    if ( _displayOn ) { // �]�w�N������������
        _lightObj.setupVertexAttributes();
        _lightObj.setShaderID(_shaderID);
        _lightObj.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
        _lightObj.setPos(_position);
    }
}

void CLight::updateToShader()
{
    //if (!_needsUpdate) return;

    GLint loc = glGetUniformLocation(_shaderID, (_lightname + ".position").c_str());
    glUniform3fv(loc, 1, glm::value_ptr(_position));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".ambient").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(_ambient));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".diffuse").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(_diffuse));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".specular").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(_specular));

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

    // _needsUpdate = false; // �C����s�᭫�]�� false
}

glm::vec3 CLight::getTarget() { return _target; }
glm::vec3 CLight::getDirection() { return _direction; }

void CLight::setTarget(const glm::vec3& target) {
    _target = target;
    _direction = glm::normalize(_target - _position);  // �p�� spot light ���ө���V
    _type = LightType::SPOT; // �]�w�� spot light
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

// �����B�ʪ��p�� 
void CLight::updateMotion(float dt) 
{ 
    // �ثe��@�H��l��m�A¶�� Y �b����A�C���� 180 �� 
    glm::mat4 mxRot; 
    glm::vec4 pos; 
    _clock += dt; 
    if (_clock >= 4.0f) _clock = 0.0f;
    // ����Y �b����x�}, �íp��s��������m
    mxRot = glm::rotate(glm::mat4(1.0f), _clock * M_PI_2, glm::vec3(0.0f, 1.0f, 0.0f));
    pos = glm::vec4(_posStart, 1.0f);
    _position = glm::vec3(mxRot * pos);
    setPos(_position);
    if (_displayOn) _lightObj.setPos(_position);
    // �p�G�O spot light�A��������ܰʦ�m�ɡA�������s�p�� direction
}

void CLight::draw()
{
    if( _displayOn ) _lightObj.draw(); // ��ܥN��������ҫ�
}

void CLight::drawRaw()
{
    if (_displayOn) _lightObj.drawRaw(); // ��ܥN��������ҫ�
}