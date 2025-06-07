#pragma once
#include <glm/glm.hpp>
#include <glew/include/GL/glew.h>
#include <string>

class CMaterial {
public:
    // �غc�l�G�w�]�����P�����Y��
    CMaterial(glm::vec4 ambient = glm::vec4(0.2f), glm::vec4 diffuse = glm::vec4(0.8f),
              glm::vec4 specular = glm::vec4(1.0f), float shininess = 32.0f );
    ~CMaterial();
    void setAmbient(glm::vec4 amb);
    glm::vec4 getAmbient();
    void setDiffuse(glm::vec4 diff);
    glm::vec4 getDiffuse();
    void setSpecular(glm::vec4 spec);
    glm::vec4 getSpecular();
    void setShininess(float shininess);
    float getShininess();
    // �N����ѼƤW�Ǩ���w shader program
    // uniformName: GLSL �������� Material struct �W�١A�Ҧp "material"
    void uploadToShader(GLuint shaderProg, std::string uniformName);
private:
    glm::vec4 _ambient;
    glm::vec4 _diffuse;
    glm::vec4 _specular;
    float     _shininess;
};