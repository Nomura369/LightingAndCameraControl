#pragma once
// CLight.h
#include <glm/glm.hpp>
#include <glew/include/GL/glew.h>
#include <string>
#include "../models/CCube.h"

class CLight {
public:
    enum LightType { POINT = 0, SPOT = 1 };
    // �غc�l�G�إ��I�����G�w�]��m (0,0,0)�B����/���g/�譱�C��A�H�α`�ΰI���
    CLight(glm::vec3 position, glm::vec4 ambient = glm::vec4(0.1f), glm::vec4 diffuse = glm::vec4(0.8f),
           glm::vec4 specular = glm::vec4(1.0f), float constant = 1.0f, float linear = 0.0f, float quadratic = 0.0f
    );  // constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f
    //// ��V���E���O�غc��
    //CLight(glm::vec3 position, glm::vec3 direction, float innerCutOffDeg = 12.5f, float outerCutOffDeg = 17.5f,
    //    glm::vec3 ambient = glm::vec3(0.1f), glm::vec3 diffuse = glm::vec3(0.8f), glm::vec3 specular = glm::vec3(1.0f),
    //    float constant = 1.0f, float linear = 0.0f, float quadratic = 0.0f
    //);
    // �ؼЦ��E���O�غc��
    CLight(glm::vec3 position, glm::vec3 target, float innerCutOffDeg = 12.5f, float outerCutOffDeg = 17.5f, float exponent = 1.0f,
           glm::vec4 ambient = glm::vec4(0.1f), glm::vec4 diffuse = glm::vec4(0.8f), glm::vec4 specular = glm::vec4(1.0f),
           float constant = 1.0f, float linear = 0.0f, float quadratic = 0.0f
    );
    ~CLight();

    // Setter / Getter
    void setPos(glm::vec3 pos);
    glm::vec3 getPos();

    void setAmbient(glm::vec4 amb);
    glm::vec4 getAmbient();

    void setDiffuse(glm::vec4 diff);
    glm::vec4 getDiffuse();

    void setSpecular(glm::vec4 spec);
    glm::vec4 getSpecular();

    void setAttenuation(float c, float l, float q);
    void getAttenuation(float &c, float &l, float &q);

    void setIntensity(float intensity);

    void setLightOn(bool enable);
    bool isLightOn();

    void setTarget(const glm::vec3& target);
    void setCutOffDeg(float innerDeg, float outerDeg, float exponent = 1.0f);
    glm::vec3 getTarget(); 
    glm::vec3 getDirection();

    // �N�Ҧ��ѼƤW�Ǩ���w shader program �� uniform struct
    // uniformName: GLSL �������� PointLight �ܼƦW�١A�Ҧp "pointLight"  
    void setShaderID(GLuint shaderProg, std::string uniformName, bool displayon=true);
    void updateToShader();
    void update(float dt);
    void setMotionEnabled(); // �]�w���}������A
    void updateMotion(float dt);

    // �yø�N�� light ���ҫ�
    void draw();
    void drawRaw();

private:
    std::string _lightname;
    GLuint _shaderID;
    glm::vec3 _position;
    glm::vec3 _posStart;
    glm::vec4 _ambient;
    glm::vec4 _diffuse;
    glm::vec4 _specular;
    float     _constant, _linear, _quadratic;
    float     _intensity;

    // Spot Light
    glm::vec3 _direction;
    glm::vec3 _target;
    float     _innerCutOff;   // cos(innerAngle)
    float     _outerCutOff;   // cos(outerAngle)
    float     _exponent;      // cos ������
    LightType _type;
    bool      _lighingOn;

    // �N�� light ���ҫ��A�i�ۦ��
    CCube _lightObj;
    bool  _displayOn;  // �O�_��ܥN��������ҫ�

    // �ʺA��������ܼ�
    float _clock;    // �ʺA��� clock
    bool  _motionOn; // �����ʺA������
};
