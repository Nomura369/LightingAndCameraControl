#include <iostream>
#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <array>

#include "CCamera.h"
#include "wmhandler.h"
#include "arcball.h"

#include "../common/CMaterial.h"
#include "../models/CQuad.h"
#include "../models/CCube.h"
#include "../models/CSphere.h"
#include "../models/CTeapot.h"
#include "../common/CLight.h"

//#define SPOT_TARGET  // Example 2 �Ѷ}

extern Arcball g_arcball;

bool  g_bCamRoting = false; // �Ω����Y������
bool  g_bfirstMouse = true;   // �ƹ����䭺�����U�A�w�]�� true
float g_lastX = 400, g_lastY = 400; // �ƹ����ʪ��Z��
float g_mouseSens = 0.005f;   // �첾���Y���F�ӫ�  

extern CCube g_centerloc;
extern GLuint g_shadingProg;
extern glm::vec3 g_eyeloc;
extern CLight g_light;

extern CMaterial g_matWaterGreen;
extern CSphere  g_sphere; 

#ifdef SPOT_TARGET
extern CCube g_spotTarget;
#endif


Arcball g_arcball; //�O�d����

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
    //glm::vec3 vPos;
    
    glm::vec3 front, up, right; // �N�����Y�����P�b�V
    float speed = 0.05f; // ���Y�첾�t��

    glm::mat4 mxView, mxProj;
    GLint viewLoc, projLoc;
    float shin;

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
            break;
        case GLFW_KEY_SPACE:
            break;
#ifdef SPOT_TARGET
        case 262:   // �V�k��
            vPos = g_spotTarget.getPos();
            g_spotTarget.setPos(glm::vec3(vPos.x + 0.15f, vPos.y, vPos.z));
            g_light.setTarget(g_spotTarget.getPos());
            break;
        case 263:   // �V����
            vPos = g_spotTarget.getPos();
            g_spotTarget.setPos(glm::vec3(vPos.x - 0.15f, vPos.y, vPos.z));
            g_light.setTarget(g_spotTarget.getPos());
            break;
        case 264:   // �V�U��
            vPos = g_spotTarget.getPos();
            g_spotTarget.setPos(glm::vec3(vPos.x, vPos.y, vPos.z - 0.15f));
            g_light.setTarget(g_spotTarget.getPos());
            break;
        case 265:   // �V�W��
            vPos = g_spotTarget.getPos();
            g_spotTarget.setPos(glm::vec3(vPos.x, vPos.y, vPos.z + 0.15f));
            g_light.setTarget(g_spotTarget.getPos());
            break;
#endif
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
                        case 'g': // ���B�ΨӴ�֧��誺 shininess, Example3 �A�W�[
                            shin = g_matWaterGreen.getShininess() - 0.5f;
                            if (shin <= 1) shin = 1;
                            //std::cout << shin << std::endl;
                            g_matWaterGreen.setShininess(shin);
                            g_sphere.setMaterial(g_matWaterGreen);
                            break;
                        case 'G': // ���B�ΨӼW�[���誺 shininess, Example3 �A�W�[
                            shin = g_matWaterGreen.getShininess() + 0.5f;
                            if (shin >= 500 ) shin = 500;
                            //std::cout << shin << std::endl;
                            g_matWaterGreen.setShininess(shin);
                            g_sphere.setMaterial(g_matWaterGreen);
                        break;
                        case 'P':
                        case 'p':
							if (CCamera::getInstance().getProjectionType() != CCamera::Type::PERSPECTIVE) {
								CCamera::getInstance().updatePerspective(45.0f, 1.0f, 1.0f, 100.0f);
                                mxProj = CCamera::getInstance().getProjectionMatrix();
                                projLoc = glGetUniformLocation(g_shadingProg, "mxProj"); // ���o projection matrix �ܼƦ�m
                                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mxProj));
                            }
                            break;
						case 'O':
						case 'o':
                            if (CCamera::getInstance().getProjectionType() != CCamera::Type::ORTHOGRAPHIC) {
								CCamera::getInstance().updateOrthographic(-3.0f, 3.0f, -3.0f, 3.0f, 1.0f, 100.0f);
                                mxProj = CCamera::getInstance().getProjectionMatrix();
                                projLoc = glGetUniformLocation(g_shadingProg, "mxProj"); // ���o projection matrix �ܼƦ�m
                                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mxProj));
                            } 
                            break;
                        case 'W':
                        case 'w':
                            // ���e����
                            g_eyeloc = CCamera::getInstance().getViewLocation(); // ���o eye ��m
                            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // center ��m�h�� g_centerloc.getPos()
                            g_eyeloc -= front * speed; // �ھ����Y�¦V��V�]front�^�i��첾
                            g_centerloc.setPos(g_centerloc.getPos() -= front * speed); // ���@�� center �� cube �ж��]��۲���
                            // ��s��v����m�P�x�}
                            CCamera::getInstance().updateViewCenter(g_eyeloc, g_centerloc.getPos());
                            mxView = CCamera::getInstance().getViewMatrix();
                            viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // ���o view matrix �ܼƪ���m
                            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                            break;
                        case 'S':
                        case 's':
                            // ���Ჾ��
                            g_eyeloc = CCamera::getInstance().getViewLocation(); // ���o eye ��m
                            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // center ��m�h�� g_centerloc.getPos()
                            g_eyeloc += front * speed; // �ھ����Y�¦V��V�]front�^�i��첾
                            g_centerloc.setPos(g_centerloc.getPos() += front * speed); // ���@�� center �� cube �ж��]��۲���
                            // ��s��v����m�P�x�}
                            CCamera::getInstance().updateViewCenter(g_eyeloc, g_centerloc.getPos());
                            mxView = CCamera::getInstance().getViewMatrix();
                            viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // ���o view matrix �ܼƪ���m
                            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                            break;
                        case 'A':
                        case 'a':
                            // ��������
                            g_eyeloc = CCamera::getInstance().getViewLocation(); // ���o eye ��m
                            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // center ��m�h�� g_centerloc.getPos()
                            up = CCamera::getInstance().getUpVector();
                            right = glm::normalize(glm::cross(front, up)); // ���o���Y�k���b�V

                            g_eyeloc += right * speed; // �ھ����Y�k���]right�^�i��첾
                            g_centerloc.setPos(g_centerloc.getPos() += right * speed); // ���@�� center �� cube �ж��]��۲���
                            // ��s��v����m�P�x�}
                            CCamera::getInstance().updateViewCenter(g_eyeloc, g_centerloc.getPos());
                            mxView = CCamera::getInstance().getViewMatrix();
                            viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // ���o view matrix �ܼƪ���m
                            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                            break;
                        case 'D':
                        case 'd':
                            // ���k����
                            g_eyeloc = CCamera::getInstance().getViewLocation(); // ���o eye ��m
                            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // center ��m�h�� g_centerloc.getPos()
                            up = CCamera::getInstance().getUpVector();
                            right = glm::normalize(glm::cross(front, up)); // ���o���Y�k���b�V

                            g_eyeloc -= right * speed; // �ھ����Y�k���]right�^�i��첾
                            g_centerloc.setPos(g_centerloc.getPos() -= right * speed); // ���@�� center �� cube �ж��]��۲���
                            // ��s��v����m�P�x�}
                            CCamera::getInstance().updateViewCenter(g_eyeloc, g_centerloc.getPos());
                            mxView = CCamera::getInstance().getViewMatrix();
                            viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // ���o view matrix �ܼƪ���m
                            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                            break;
                        case 'L':
                        case 'l':
                            // ���I�����i���P�B��
                            g_light.setMotionEnabled();
                            break;
                    }
                }   
            }
        
    }
}