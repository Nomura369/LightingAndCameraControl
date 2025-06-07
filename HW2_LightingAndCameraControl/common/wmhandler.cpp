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

//#define SPOT_TARGET  // Example 2 解開

extern Arcball g_arcball;

bool  g_bCamRoting = false; // 用於鏡頭的旋轉
bool  g_bfirstMouse = true;   // 滑鼠左鍵首次按下，預設為 true
float g_lastX = 400, g_lastY = 400; // 滑鼠移動的距離
float g_mouseSens = 0.005f;   // 位移鏡頭的靈敏度  

extern CCube g_centerloc;
extern GLuint g_shadingProg;
extern glm::vec3 g_eyeloc;
extern CLight g_light;

extern CMaterial g_matWaterGreen;
extern CSphere  g_sphere; 

#ifdef SPOT_TARGET
extern CCube g_spotTarget;
#endif


Arcball g_arcball; //保留未用

// 滑鼠按鈕按下後 callback function(回呼函式) ---------------
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
			g_bfirstMouse = true; // 滑鼠釋放時，重新設定滑鼠左鍵尚未按下
        }      
    }
}
// ---------------------------------------------------------------------------------------

// 滑鼠在指定視窗上移動時的 callback function (回呼函式)
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    g_arcball.onCursorMove(xpos, ypos, width, height);
    // Arcball* arcball = static_cast<Arcball*>(glfwGetWindowUserPointer(window));
    //std::cout << "x = " << xpos << "y = " << ypos << std::endl;
    if ( g_bCamRoting )  // 進入控制鏡頭模式
    {
        if (g_bfirstMouse) {  // 左鍵首次按下
            g_lastX = (float)xpos; g_lastY = (float)ypos; g_bfirstMouse = false;
            return;
        }
        float deltaX = ((float)xpos - g_lastX);
        float deltaY = ((float)ypos - g_lastY);
        g_lastX = (float)xpos; g_lastY = (float)ypos;

        CCamera::getInstance().processMouseMovement(deltaX, deltaY, g_mouseSens);
        glm::mat4 mxView = CCamera::getInstance().getViewMatrix();
        GLint viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); 	// 取得 view matrix 變數位置
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
        g_eyeloc = CCamera::getInstance().getViewLocation();
    }
}
// ---------------------------------------------------------------------------------------
// 當視窗大小改變的 callback function
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// ---------------------------------------------------------------------------------------
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // yoffset 滑鼠滾輪移動的垂直位移
    // 以 yoffset 來改變 eye 與 center 間的距離，改變 radius 即可

    CCamera::getInstance().updateRadius((float)yoffset * -0.2f);
    glm::mat4 mxView = CCamera::getInstance().getViewMatrix();
    GLint viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); 	// 取得 mxView 變數的位置
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
    g_eyeloc = CCamera::getInstance().getViewLocation();

    //std::cout << "Scroll event: xoffset = " << xoffset << ", yoffset = " << yoffset << std::endl;
}

// 當鍵盤按鍵按下時的 callback function (回呼函式)
// key : GLFW_KEY_0、 GLFW_KEY_a、GLFW_KEY_ESCAPE、GLFW_KEY_SPACE
//       完整定義在這邊 https://www.glfw.org/docs/latest/group__keys.html
// action : 
//          GLFW_PRESS：按鍵被按下。
//          GLFW_RELEASE：按鍵被釋放。
//          GLFW_REPEAT：按鍵被重複觸發（長按時會觸發多次）
// mods : 
//          GLFW_MOD_SHIFT：Shift 鍵被按下。
//          GLFW_MOD_CONTROL：Ctrl 鍵被按下。
//          GLFW_MOD_ALT：Alt 鍵被按下。
//          GLFW_MOD_SUPER：Windows 鍵或 Command 鍵被按下。
//          GLFW_MOD_CAPS_LOCK：Caps Lock 鍵被啟用。
//          GLFW_MOD_NUM_LOCK：Num Lock 鍵被啟用。
// 
//  注意：1. 一個按鍵按下到放開會產生兩次 keyCallback，
//           根據功能需求判斷是按下或是放掉去執行對應的功能
//        2. 使用 glfw 英文字母沒有分大小寫，都是傳回大寫的 ASCII 編碼
//           搭配檢查左右 shift 鍵是否按下是大寫還是小寫(假設 caps 鍵沒有被按下)
//       
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //glm::vec3 vPos;
    
    glm::vec3 front, up, right; // 代表鏡頭的不同軸向
    float speed = 0.05f; // 鏡頭位移速度

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
        case 262:   // 向右鍵
            vPos = g_spotTarget.getPos();
            g_spotTarget.setPos(glm::vec3(vPos.x + 0.15f, vPos.y, vPos.z));
            g_light.setTarget(g_spotTarget.getPos());
            break;
        case 263:   // 向左鍵
            vPos = g_spotTarget.getPos();
            g_spotTarget.setPos(glm::vec3(vPos.x - 0.15f, vPos.y, vPos.z));
            g_light.setTarget(g_spotTarget.getPos());
            break;
        case 264:   // 向下鍵
            vPos = g_spotTarget.getPos();
            g_spotTarget.setPos(glm::vec3(vPos.x, vPos.y, vPos.z - 0.15f));
            g_light.setTarget(g_spotTarget.getPos());
            break;
        case 265:   // 向上鍵
            vPos = g_spotTarget.getPos();
            g_spotTarget.setPos(glm::vec3(vPos.x, vPos.y, vPos.z + 0.15f));
            g_light.setTarget(g_spotTarget.getPos());
            break;
#endif
        default: // 針對英文字母大小寫進行處理
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                // 檢查 Shift 鍵(左右兩邊各一個)是否被按下
                bool isShiftPressed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ||
                                      (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
                // 判斷字母鍵的大小寫，預設使用鍵盤是沒有按下 CAPS 鍵
                if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
                    char letter = (isShiftPressed) ? ('A' + (key - GLFW_KEY_A)) : ('a' + (key - GLFW_KEY_A));
                    //std::cout << "key = " << letter << std::endl;
                    switch (letter) {
                        case 'g': // 此處用來減少材質的 shininess, Example3 再增加
                            shin = g_matWaterGreen.getShininess() - 0.5f;
                            if (shin <= 1) shin = 1;
                            //std::cout << shin << std::endl;
                            g_matWaterGreen.setShininess(shin);
                            g_sphere.setMaterial(g_matWaterGreen);
                            break;
                        case 'G': // 此處用來增加材質的 shininess, Example3 再增加
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
                                projLoc = glGetUniformLocation(g_shadingProg, "mxProj"); // 取得 projection matrix 變數位置
                                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mxProj));
                            }
                            break;
						case 'O':
						case 'o':
                            if (CCamera::getInstance().getProjectionType() != CCamera::Type::ORTHOGRAPHIC) {
								CCamera::getInstance().updateOrthographic(-3.0f, 3.0f, -3.0f, 3.0f, 1.0f, 100.0f);
                                mxProj = CCamera::getInstance().getProjectionMatrix();
                                projLoc = glGetUniformLocation(g_shadingProg, "mxProj"); // 取得 projection matrix 變數位置
                                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mxProj));
                            } 
                            break;
                        case 'W':
                        case 'w':
                            // 往前移動
                            g_eyeloc = CCamera::getInstance().getViewLocation(); // 取得 eye 位置
                            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // center 位置則為 g_centerloc.getPos()
                            g_eyeloc -= front * speed; // 根據鏡頭朝向方向（front）進行位移
                            g_centerloc.setPos(g_centerloc.getPos() -= front * speed); // 讓作為 center 的 cube 房間也跟著移動
                            // 更新攝影機位置與矩陣
                            CCamera::getInstance().updateViewCenter(g_eyeloc, g_centerloc.getPos());
                            mxView = CCamera::getInstance().getViewMatrix();
                            viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // 取得 view matrix 變數的位置
                            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                            break;
                        case 'S':
                        case 's':
                            // 往後移動
                            g_eyeloc = CCamera::getInstance().getViewLocation(); // 取得 eye 位置
                            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // center 位置則為 g_centerloc.getPos()
                            g_eyeloc += front * speed; // 根據鏡頭朝向方向（front）進行位移
                            g_centerloc.setPos(g_centerloc.getPos() += front * speed); // 讓作為 center 的 cube 房間也跟著移動
                            // 更新攝影機位置與矩陣
                            CCamera::getInstance().updateViewCenter(g_eyeloc, g_centerloc.getPos());
                            mxView = CCamera::getInstance().getViewMatrix();
                            viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // 取得 view matrix 變數的位置
                            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                            break;
                        case 'A':
                        case 'a':
                            // 往左移動
                            g_eyeloc = CCamera::getInstance().getViewLocation(); // 取得 eye 位置
                            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // center 位置則為 g_centerloc.getPos()
                            up = CCamera::getInstance().getUpVector();
                            right = glm::normalize(glm::cross(front, up)); // 取得鏡頭右側軸向

                            g_eyeloc += right * speed; // 根據鏡頭右側（right）進行位移
                            g_centerloc.setPos(g_centerloc.getPos() += right * speed); // 讓作為 center 的 cube 房間也跟著移動
                            // 更新攝影機位置與矩陣
                            CCamera::getInstance().updateViewCenter(g_eyeloc, g_centerloc.getPos());
                            mxView = CCamera::getInstance().getViewMatrix();
                            viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // 取得 view matrix 變數的位置
                            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                            break;
                        case 'D':
                        case 'd':
                            // 往右移動
                            g_eyeloc = CCamera::getInstance().getViewLocation(); // 取得 eye 位置
                            front = glm::normalize(g_eyeloc - g_centerloc.getPos()); // center 位置則為 g_centerloc.getPos()
                            up = CCamera::getInstance().getUpVector();
                            right = glm::normalize(glm::cross(front, up)); // 取得鏡頭右側軸向

                            g_eyeloc -= right * speed; // 根據鏡頭右側（right）進行位移
                            g_centerloc.setPos(g_centerloc.getPos() -= right * speed); // 讓作為 center 的 cube 房間也跟著移動
                            // 更新攝影機位置與矩陣
                            CCamera::getInstance().updateViewCenter(g_eyeloc, g_centerloc.getPos());
                            mxView = CCamera::getInstance().getViewMatrix();
                            viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // 取得 view matrix 變數的位置
                            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                            break;
                        case 'L':
                        case 'l':
                            // 讓點光源進行圓周運動
                            g_light.setMotionEnabled();
                            break;
                    }
                }   
            }
        
    }
}