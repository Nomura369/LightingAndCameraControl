#include <iostream>
#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <array>

#include "CCamera.h"
#include "wmhandler.h"
#include "arcball.h"
#include "CMaterial.h"
#include "CLight.h"
#include "CButton.h"

#include "../models/CQuad.h"
#include "../models/CCube.h"
#include "../models/CSphere.h"
#include "../models/CTeapot.h"

//#define SPOT_TARGET

extern Arcball g_arcball;

bool  g_bCamRoting = false; // �Ω����Y������
bool  g_bfirstMouse = true;   // �ƹ����䭺�����U�A�w�]�� true
float g_lastX = 400, g_lastY = 400; // �ƹ����ʪ��Z��
float g_mouseSens = 0.005f;   // �첾���Y���F�ӫ�
bool g_isCameraBasedMoving = true; // �첾�O�H���Y�y�� or �@�ɮy�Ь���ǡ]�w�]���e�̡^

extern CCube g_centerloc;
extern GLuint g_shadingProg;
extern glm::vec3 g_eyeloc;
extern CLight g_light;
extern CLight g_capSpotLight;
extern CLight g_cupSpotLight;
extern CLight g_knotSpotLight;

extern CMaterial g_matWaterGreen;
extern CSphere  g_sphere; 

extern std::array<CButton, 4> g_button;
extern bool g_isNpr;
extern bool g_isGradient;
extern float g_colorTime;

Arcball g_arcball; //�O�d����

// �ж���ɳ]�w
struct RoomAABB {
    glm::vec3 roomMin; // �ж������U���]�γ̧C�I�^
    glm::vec3 roomMax; // �ж����k�W���]�γ̰��I�^

    bool isInsideRoom(const glm::vec3& pos) {
        if ((pos.x >= roomMin.x && pos.x <= roomMax.x) &&
            (pos.y >= roomMin.y && pos.y <= roomMax.y) &&
            (pos.z >= roomMin.z && pos.z <= roomMax.z)) {
            return true;
        }
        else {
            std::cout << "�I����F�]�p�G�����䥢�F�i�H���Y���A�դ@���^" << std::endl << std::endl;
            return false;
        }  
    }
};
RoomAABB theRoom = {
    glm::vec3(-15.0f, -15.0f, -11.0f), // roomMin
    glm::vec3(15.0f, 15.0f, 11.0f), // roomMax
};

// �첾�Ψ禡�]���M�{���X�����F�^
void moveForward(bool isForward) {
    glm::vec3 vPos;
    glm::vec3 front; // �N�����Y�����P�b�V
    float speed = 0.05f; // ���Y�첾�t��
    glm::vec3 eyeloc, centerloc; // �Ȧs g_eyeloc �M g_centerloc �H��K�p��
    glm::mat4 mxView;
    GLint viewLoc;

    if (isForward) { // �V�e����
        if (g_isCameraBasedMoving) {
            // ���Y��V���D
            g_eyeloc = CCamera::getInstance().getViewLocation(); // ���o eye ��m
            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // front �������ݦV���ߤ�V
            eyeloc = g_eyeloc - front * speed;
            centerloc = g_centerloc.getPos() - front * speed;

            if (theRoom.isInsideRoom(eyeloc) && theRoom.isInsideRoom(centerloc)) {
                g_eyeloc = eyeloc;
                g_centerloc.setPos(centerloc);
            }
        }
        else {
            // �@�ɮy�Ф�V���D
            vPos = g_centerloc.getPos();
            g_centerloc.setPos(glm::vec3(vPos.x, vPos.y, vPos.z - speed));
            g_eyeloc = CCamera::getInstance().getViewLocation();
            g_eyeloc.z -= speed;

            if (theRoom.isInsideRoom(g_eyeloc) && theRoom.isInsideRoom(g_centerloc.getPos())) {
                // �S���D�N�O�d����
            }
            else {
                // �Y���b�ж����A�٭��m
                g_centerloc.setPos(vPos);
                g_eyeloc = CCamera::getInstance().getViewLocation();
            }
        }
    }
    else { // �V�Ჾ��
        if (g_isCameraBasedMoving) {
            // ���Y��V���D
            g_eyeloc = CCamera::getInstance().getViewLocation(); // ���o eye ��m
            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // front �������ݦV���ߤ�V
            eyeloc = g_eyeloc + front * speed;
            centerloc = g_centerloc.getPos() + front * speed;

            if (theRoom.isInsideRoom(eyeloc) && theRoom.isInsideRoom(centerloc)) {
                g_eyeloc = eyeloc;
                g_centerloc.setPos(centerloc);
            }
        }
        else {
            // �@�ɮy�Ф�V���D
            vPos = g_centerloc.getPos();
            g_centerloc.setPos(glm::vec3(vPos.x, vPos.y, vPos.z + speed));
            g_eyeloc = CCamera::getInstance().getViewLocation();
            g_eyeloc.z += speed;

            if (theRoom.isInsideRoom(g_eyeloc) && theRoom.isInsideRoom(g_centerloc.getPos())) {
                // �S���D�N�O�d����
            }
            else {
                // �Y���b�ж����A�٭��m
                g_centerloc.setPos(vPos);
                g_eyeloc = CCamera::getInstance().getViewLocation();
            }
        }
    }

    // �@�q��s��v���P view matrix
    CCamera::getInstance().updateViewCenter(g_eyeloc, g_centerloc.getPos());
    mxView = CCamera::getInstance().getViewMatrix();
    viewLoc = glGetUniformLocation(g_shadingProg, "mxView");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
}
void moveRight(bool isRight) {
    glm::vec3 vPos;
    glm::vec3 front, up, right; // �N�����Y�����P�b�V
    float speed = 0.05f; // ���Y�첾�t��
    glm::vec3 eyeloc, centerloc; // �Ȧs g_eyeloc �M g_centerloc �H��K�p��
    glm::mat4 mxView;
    GLint viewLoc;

    if (isRight) { // �V�k����
        if (g_isCameraBasedMoving) {
            // ���Y��V���D
            g_eyeloc = CCamera::getInstance().getViewLocation(); // eye
            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // front �V�q
            up = CCamera::getInstance().getUpVector(); // up �V�q
            right = glm::normalize(glm::cross(front, up)); // �k��V�V�q

            eyeloc = g_eyeloc - right * speed;
            centerloc = g_centerloc.getPos() - right * speed;

            if (theRoom.isInsideRoom(eyeloc) && theRoom.isInsideRoom(centerloc)) {
                g_eyeloc = eyeloc;
                g_centerloc.setPos(centerloc);
            }
        }
        else {
            // �@�ɮy�Ф�V���D
            vPos = g_centerloc.getPos();
            g_centerloc.setPos(glm::vec3(vPos.x + speed, vPos.y, vPos.z));
            g_eyeloc = CCamera::getInstance().getViewLocation();
            g_eyeloc.x += speed;

            if (theRoom.isInsideRoom(g_eyeloc) && theRoom.isInsideRoom(g_centerloc.getPos())) {
                // �O�d
            }
            else {
                // �Y�|����h�٭�
                g_centerloc.setPos(vPos);
                g_eyeloc = CCamera::getInstance().getViewLocation();
            }
        }
    }
    else { // �V������
        if (g_isCameraBasedMoving) {
            // ���Y��V���D
            g_eyeloc = CCamera::getInstance().getViewLocation(); // eye
            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // front �V�q
            up = CCamera::getInstance().getUpVector(); // up �V�q
            right = glm::normalize(glm::cross(front, up)); // �k��V�V�q

            eyeloc = g_eyeloc + right * speed;
            centerloc = g_centerloc.getPos() + right * speed;

            if (theRoom.isInsideRoom(eyeloc) && theRoom.isInsideRoom(centerloc)) {
                g_eyeloc = eyeloc;
                g_centerloc.setPos(centerloc);
            }
        }
        else {
            // �@�ɮy�Ф�V���D
            vPos = g_centerloc.getPos();
            g_centerloc.setPos(glm::vec3(vPos.x - speed, vPos.y, vPos.z));
            g_eyeloc = CCamera::getInstance().getViewLocation();
            g_eyeloc.x -= speed;

            if (theRoom.isInsideRoom(g_eyeloc) && theRoom.isInsideRoom(g_centerloc.getPos())) {
                // �O�d
            }
            else {
                // �Y�|����h�٭�
                g_centerloc.setPos(vPos);
                g_eyeloc = CCamera::getInstance().getViewLocation();
            }
        }
    }

    // �@�q��s��v���P view matrix
    CCamera::getInstance().updateViewCenter(g_eyeloc, g_centerloc.getPos());
    mxView = CCamera::getInstance().getViewMatrix();
    viewLoc = glGetUniformLocation(g_shadingProg, "mxView");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
}


// �ƹ����s���U�� callback function(�^�I�禡) ---------------
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    // Arcball* arcball = static_cast<Arcball*>(glfwGetWindowUserPointer(window));
    g_arcball.onMouseButton(button, action, xpos, ypos);
    //std::cout << "button = " << button << "action = " << action << "mods = " << mods << std::endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT )
    {
		if (action == GLFW_PRESS)
		{
            g_bCamRoting = true;
            // �Ұʩ��������s��
            if (g_button[0].handleClick((float)xpos, height - (float)ypos)) {  }
            if (g_button[1].handleClick((float)xpos, height - (float)ypos)) {  }
            if (g_button[2].handleClick((float)xpos, height - (float)ypos)) {  }
            if (g_button[3].handleClick((float)xpos, height - (float)ypos)) {  }
		}
        else if (action == GLFW_RELEASE)
        {
            g_bCamRoting = false; 
			g_bfirstMouse = true; // �ƹ�����ɡA���s�]�w�ƹ�����|�����U
        }      
    }
}
// ---------------------------------------------------------------------------------------

// �ƹ��b���w�����W���ʮɪ� callback function (�^�I�禡)
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    g_arcball.onCursorMove(xpos, ypos, width, height);
    // Arcball* arcball = static_cast<Arcball*>(glfwGetWindowUserPointer(window));
    //std::cout << "x = " << xpos << "y = " << ypos << std::endl;
    if ( g_bCamRoting )  // �i�J�������Y�Ҧ�
    {
        if (g_bfirstMouse) {  // ���䭺�����U
            g_lastX = (float)xpos; g_lastY = (float)ypos; g_bfirstMouse = false;
            return;
        }
        float deltaX = ((float)xpos - g_lastX);
        float deltaY = ((float)ypos - g_lastY);
        g_lastX = (float)xpos; g_lastY = (float)ypos;

        CCamera::getInstance().processMouseMovement(deltaX, deltaY, g_mouseSens);
        glm::mat4 mxView = CCamera::getInstance().getViewMatrix();
        GLint viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); 	// ���o view matrix �ܼƦ�m
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
        g_eyeloc = CCamera::getInstance().getViewLocation();
    }
}
// ---------------------------------------------------------------------------------------
// ������j�p���ܪ� callback function
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// ---------------------------------------------------------------------------------------
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // yoffset �ƹ��u�����ʪ������첾
    // �H yoffset �ӧ��� eye �P center �����Z���A���� radius �Y�i

    CCamera::getInstance().updateRadius((float)yoffset * -0.2f);
    glm::mat4 mxView = CCamera::getInstance().getViewMatrix();
    GLint viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); 	// ���o mxView �ܼƪ���m
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
    g_eyeloc = CCamera::getInstance().getViewLocation();

    //std::cout << "Scroll event: xoffset = " << xoffset << ", yoffset = " << yoffset << std::endl;
}

// ����L������U�ɪ� callback function (�^�I�禡)
// key : GLFW_KEY_0�B GLFW_KEY_a�BGLFW_KEY_ESCAPE�BGLFW_KEY_SPACE
//       ����w�q�b�o�� https://www.glfw.org/docs/latest/group__keys.html
// action : 
//          GLFW_PRESS�G����Q���U�C
//          GLFW_RELEASE�G����Q����C
//          GLFW_REPEAT�G����Q����Ĳ�o�]�����ɷ|Ĳ�o�h���^
// mods : 
//          GLFW_MOD_SHIFT�GShift ��Q���U�C
//          GLFW_MOD_CONTROL�GCtrl ��Q���U�C
//          GLFW_MOD_ALT�GAlt ��Q���U�C
//          GLFW_MOD_SUPER�GWindows ��� Command ��Q���U�C
//          GLFW_MOD_CAPS_LOCK�GCaps Lock ��Q�ҥΡC
//          GLFW_MOD_NUM_LOCK�GNum Lock ��Q�ҥΡC
// 
//  �`�N�G1. �@�ӫ�����U���}�|���ͨ⦸ keyCallback�A
//           �ھڥ\��ݨD�P�_�O���U�άO�񱼥h����������\��
//        2. �ϥ� glfw �^��r���S�����j�p�g�A���O�Ǧ^�j�g�� ASCII �s�X
//           �f�t�ˬd���k shift ��O�_���U�O�j�g�٬O�p�g(���] caps ��S���Q���U)
//       
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // ����t�ө��]�̷� ambient�Bdiffuse �M specular �����ǡ^
    glm::vec4 reds[3] = { glm::vec4(0.15f, 0.05f, 0.05f, 1.0f),
                          glm::vec4(0.8f, 0.2f, 0.2f, 1.0f),
                          glm::vec4(0.9f, 0.4f, 0.4f, 1.0f)
    };
    // ���t�ө�
    glm::vec4 greens[3] = { glm::vec4(0.05f, 0.15f, 0.05f, 1.0f),
                            glm::vec4(0.2f, 0.8f, 0.2f, 1.0f),
                            glm::vec4(0.4f, 0.9f, 0.4f, 1.0f)
    };
    // �Ŧ�t�ө�
    glm::vec4 blues[3] = { glm::vec4(0.05f, 0.05f, 0.15f, 1.0f),
                           glm::vec4(0.2f, 0.2f, 0.8f, 1.0f),
                           glm::vec4(0.4f, 0.4f, 0.9f, 1.0f)
    };

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
            break;
        case GLFW_KEY_SPACE:
            break;
        default: // �w��^��r���j�p�g�i��B�z
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                // �ˬd Shift ��(���k����U�@��)�O�_�Q���U
                bool isShiftPressed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ||
                                      (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
                // �P�_�r���䪺�j�p�g�A�w�]�ϥ���L�O�S�����U CAPS ��
                if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
                    char letter = (isShiftPressed) ? ('A' + (key - GLFW_KEY_A)) : ('a' + (key - GLFW_KEY_A));
                    //std::cout << "key = " << letter << std::endl;
                    switch (letter) {
                        case 'W':
                        case 'w':
                            moveForward(true);
                            break;
                        case 'S':
                        case 's':
                            moveForward(false);
                            break;
                        case 'A':
                        case 'a':
                            moveRight(false);
                            break;
                        case 'D':
                        case 'd':
                            moveRight(true);
                            break;
                        case 'N':
                        case 'n':
                            // �����ө�����]�O�_���d�q�^
                            g_isNpr = !g_isNpr;
                            if (g_isNpr) {
                                std::cout << "�ثe�� NPR �Ҧ�" << std::endl << std::endl;
                            }
                            else {
                                std::cout << "�ثe�� Per-Pixel Lighting �Ҧ�" << std::endl << std::endl;
                            }
                            break;
                        case 'C':
                        case 'c':
                            // �����첾����
                            g_isCameraBasedMoving = !g_isCameraBasedMoving;
                            if (g_isCameraBasedMoving) {
                                std::cout << "�ثe�H�u���Y��V�v�i��첾" << std::endl << std::endl;
                            }
                            else {
                                std::cout << "�ثe�H�u�@�ɮy�Ф�V�v�i��첾" << std::endl << std::endl;
                            }
                            break;
                        case 'R':
                        case 'r':
                            if (!g_isGradient) {
                                // �N�ж���������t�ө�
                                g_light.setAmbient(reds[0]);
                                g_light.setDiffuse(reds[1]);
                                g_light.setSpecular(reds[2]);
                                std::cout << "����t�ж�" << std::endl << std::endl;
                            }
                            break;
                        case 'G':
                        case 'g':
                            if (!g_isGradient) {
                                // �N�ж��������t�ө�
                                g_light.setAmbient(greens[0]);
                                g_light.setDiffuse(greens[1]);
                                g_light.setSpecular(greens[2]);
                                std::cout << "���t�ж�" << std::endl << std::endl;
                            }
                            break;
                        case 'B':
                        case 'b':
                            if (!g_isGradient) {
                                // �N�ж������Ŧ�t�ө�
                                g_light.setAmbient(blues[0]);
                                g_light.setDiffuse(blues[1]);
                                g_light.setSpecular(blues[2]);
                                std::cout << "�Ŧ�t�ж�" << std::endl << std::endl;
                            }
                            break;
                        case 'H':
                        case 'h':
                            // �N�ж������w�]�ө�
                            if (!g_isGradient) {
                                g_light.setAmbient(glm::vec4(0.1f));
                                g_light.setDiffuse(glm::vec4(0.8f));
                                g_light.setSpecular(glm::vec4(1.0f));
                                std::cout << "�w�]�ж�" << std::endl << std::endl;
                            }
                            break;
                        case 'L':
                        case 'l':
                            // �O�_���ܷө���m
                            g_isGradient = !g_isGradient;
                            if (!g_isGradient) {
                                g_colorTime = 0.0f;
                                std::cout << "�����۰��ܦ�" << std::endl << std::endl;
                            }
                            else std::cout << "�}�Ҧ۰��ܦ�" << std::endl << std::endl;
                            break;
                    }
                }   
            }
        
    }
}