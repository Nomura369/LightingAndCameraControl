//#define GLM_ENABLE_EXPERIMENTAL 1

#include <iostream>
#include <fstream>
#include <sstream>

#include <glew/include/GL/glew.h>
#include <glfw/include/GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>

#include "common/initshader.h"
#include "common/arcball.h"
#include "common/wmhandler.h"
#include "common/CCamera.h"
#include "common/CShaderPool.h"
#include "../models/CQuad.h"
#include "../models/CTorusKnot.h"
#include "../models/CBox.h"
#include "../models/CSphere.h"
#include "../models/CCapsule.h"
#include "../models/CCup.h"

#include "common/CLight.h"
#include "common/CMaterial.h"
#include "../models/CObjModel.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800 
#define ROW_NUM 30

// 範例模型宣告區
CCapsule g_capsule;
CCup g_cup;
CTorusKnot g_knot(4);
CBox g_house;
CSphere g_sphere;

// obj 檔模型宣告區
CObjModel g_objModel;

// 鏡頭與場景宣告區
glm::vec3 g_eyeloc(6.0f, 6.0f, 6.0f); // 鏡頭位置, 預設在 (8,8,8) 
CCube g_centerloc; // view center預設在 (0,0,0)，不做任何描繪操作
CQuad g_floor[ROW_NUM][ROW_NUM]; 

GLuint g_shadingProg;

// 全域光源 (位置在 5,5,0)
CLight g_light(glm::vec3(5.0f, 5.0f, 0.0f));

// 全域材質（可依模型分別設定）
CMaterial g_matBeige;   // 淺米白
CMaterial g_matGray;    //  深麥灰材質
CMaterial g_matWaterBlue;
CMaterial g_matWaterGreen;
CMaterial g_matWaterRed;
CMaterial g_matWoodHoney;
CMaterial g_matWoodLightOak;
CMaterial g_matWoodBleached;

void genMaterial();

//----------------------------------------------------------------------------
void loadScene(void)
{
    genMaterial();
    g_shadingProg = CShaderPool::getInstance().getShader("v_npr.glsl", "f_npr.glsl");   
    g_light.setShaderID(g_shadingProg, "uLight");
    //g_light.setTarget(glm::vec3(0, 2, 0));
    //g_light.setCutOffDeg(20.0f, 90.0f);
    //g_light.setCutOffDeg(20.0f, 90.0f, 8.0f);

    int k = 0;
    for (int i = 0; i < ROW_NUM; i++)
    {
        k++;
        for (int j = 0; j < ROW_NUM; j++) {
            g_floor[i][j].setupVertexAttributes();
            g_floor[i][j].setShaderID(g_shadingProg, 3);
            g_floor[i][j].setPos(glm::vec3((ROW_NUM / 2) - 0.5f - (float)i, 0.0f, (float)j - (ROW_NUM / 2) + 0.5f));
            g_floor[i][j].setRotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            if (k % 2) g_floor[i][j].setMaterial(g_matBeige);
            else g_floor[i][j].setMaterial(g_matGray);
            k++;
        }
    }

    g_capsule.setupVertexAttributes();
    g_capsule.setShaderID(g_shadingProg, 3);
    g_capsule.setPos(glm::vec3(8.0f, 0.5f, -8.0f));
    g_capsule.setRotate(90, glm::vec3(1, 0, 0));
    g_capsule.setMaterial(g_matWaterRed);

    g_cup.setupVertexAttributes();
    g_cup.setShaderID(g_shadingProg, 3);
    g_cup.setPos(glm::vec3(-8.0f, 0.5f, -8.0f));
    g_cup.setMaterial(g_matWaterBlue);

    g_objModel.setupVertexAttributes();
    g_objModel.setShaderID(g_shadingProg, 3);
    g_objModel.setScale(glm::vec3(5.0f, 5.0f, 5.0f));
    g_objModel.setPos(glm::vec3(0.0f, 0.0005f, 0.0f)); // 房間正中間
    g_objModel.setMaterial(g_matWoodLightOak);
 
    g_knot.setupVertexAttributes();
    g_knot.setShaderID(g_shadingProg, 3);
    g_knot.setScale(glm::vec3(0.4f, 0.4f, 0.4f));
    g_knot.setPos(glm::vec3(0.0f, 0.5f, 8.0f));
    g_knot.setMaterial(g_matWaterGreen);

    g_house.setupVertexAttributes();
    g_house.setShaderID(g_shadingProg, 3);
    g_house.setScale(glm::vec3(30.0f, 12.0f, 30.0f));
    g_house.setPos(glm::vec3(0.0f, 5.95f, 0.0f));
    g_house.setMaterial(g_matWoodBleached);

	CCamera::getInstance().updateView(g_eyeloc); // 設定 eye 位置
    CCamera::getInstance().updateCenter(glm::vec3(0,4,0));
	CCamera::getInstance().updatePerspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 mxView = CCamera::getInstance().getViewMatrix();
	glm::mat4 mxProj = CCamera::getInstance().getProjectionMatrix();

    GLint viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); 	// 取得 view matrix 變數的位置
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));

    GLint projLoc = glGetUniformLocation(g_shadingProg, "mxProj"); 	// 取得投影矩陣變數的位置
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mxProj));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 設定清除 back buffer 背景的顏色
    glEnable(GL_DEPTH_TEST); // 啟動深度測試
}
//----------------------------------------------------------------------------

void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 設定 back buffer 的背景顏色
    glUseProgram(g_shadingProg);

    //上傳光源與相機位置
    g_light.updateToShader();
    glUniform3fv(glGetUniformLocation(g_shadingProg, "viewPos"), 1, glm::value_ptr(g_eyeloc));
    glUniform3fv(glGetUniformLocation(g_shadingProg, "lightPos"), 1, glm::value_ptr(g_light.getPos()));

    for (int i = 0; i < ROW_NUM; i++)
        for (int j = 0; j < ROW_NUM; j++) {
            g_floor[i][j].uploadMaterial();
            g_floor[i][j].drawRaw();
        }

    g_light.drawRaw();
    
    g_capsule.uploadMaterial();
    g_capsule.drawRaw();
    g_cup.uploadMaterial();
    g_cup.drawRaw();
    g_objModel.uploadMaterial();
    g_objModel.drawRaw();
    g_knot.uploadMaterial();
    g_knot.drawRaw();

    g_house.uploadMaterial();
    g_house.drawRaw();
}
//----------------------------------------------------------------------------

void update(float dt)
{
    g_light.update(dt);
}

void releaseAll()
{

}

int main() {
    // ------- 檢查與建立視窗  ---------------  
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 設定 OpenGL 版本與 Core Profile
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //只啟用 OpenGL 3.3 Core Profile（不包含舊版 OpenGL 功能）
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 禁止視窗大小改變

    // 建立 OpenGL 視窗與該視窗執行時所需的的狀態、資源和環境(context 上下文)
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL_4 Example 4 NPR", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 設定將這個視窗的資源(OpenGL 的圖形上下文）與當前執行緒綁定，讓該執行緒能夠操作該視窗的資源
    glfwMakeContextCurrent(window);

    // 設定視窗大小, 這樣 OpenGL 才能知道如何將視窗的內容繪製到正確的位置
    // 基本上寬與高設定成視窗的寬與高即可
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    // ---------------------------------------

    // 設定相關事件的 callback 函式，以便事件發生時，能呼叫對應的函式
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);// 視窗大小被改變時
    glfwSetKeyCallback(window, keyCallback);                        // 有鍵盤的按鍵被按下時
    glfwSetMouseButtonCallback(window, mouseButtonCallback);        // 有滑鼠的按鍵被按下時
    glfwSetCursorPosCallback(window, cursorPosCallback);            // 滑鼠在指定的視窗上面移動時
	glfwSetScrollCallback(window, scrollCallback);			        // 滑鼠滾輪滾動時

    // 呼叫 loadScene() 建立與載入 GPU 進行描繪的幾何資料 
    loadScene();

    float lastTime = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime; // 計算前一個 frame 到目前為止經過的時間
        lastTime = currentTime;
        update(deltaTime);      // 呼叫 update 函式，並將 deltaTime 傳入，讓所有動態物件根據時間更新相關內容
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    releaseAll(); // 程式結束前釋放所有的資源
    glfwTerminate();
    return 0;
}

void genMaterial()
{
    // 設定材質
    g_matBeige.setAmbient(glm::vec4(0.0918f, 0.0906f, 0.0863f, 1.0f));
    g_matBeige.setDiffuse(glm::vec4(0.8258f, 0.8152f, 0.7765f, 1.0f));
    g_matBeige.setSpecular(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
    g_matBeige.setShininess(32.0f);

    g_matGray.setAmbient(glm::vec4(0.0690f, 0.06196f, 0.05451f, 1.0f));
    g_matGray.setDiffuse(glm::vec4(0.6212f, 0.5576f, 0.4906f, 1.0f));
    g_matGray.setSpecular(glm::vec4(0.20f, 0.20f, 0.20f, 1.0f));
    g_matGray.setShininess(16.0f);

    g_matWaterBlue.setAmbient(glm::vec4(0.080f, 0.105f, 0.120f, 1.0f));
    g_matWaterBlue.setDiffuse(glm::vec4(0.560f, 0.740f, 0.840f, 1.0f));
    g_matWaterBlue.setSpecular(glm::vec4(0.20f, 0.20f, 0.20f, 1.0f));
    g_matWaterBlue.setShininess(32.0f);

    g_matWaterGreen.setAmbient(glm::vec4(0.075f, 0.120f, 0.090f, 1.0f));
    g_matWaterGreen.setDiffuse(glm::vec4(0.540f, 0.840f, 0.640f, 1.0f));
    g_matWaterGreen.setSpecular(glm::vec4(0.20f, 0.20f, 0.20f, 1.0f));
    g_matWaterGreen.setShininess(32.0f);

    g_matWaterRed.setAmbient(glm::vec4(0.125f, 0.075f, 0.075f, 1.0f));
    g_matWaterRed.setDiffuse(glm::vec4(0.860f, 0.540f, 0.540f, 1.0f));
    g_matWaterRed.setSpecular(glm::vec4(0.20f, 0.20f, 0.20f, 1.0f));
    g_matWaterRed.setShininess(32.0f);

    g_matWoodHoney.setAmbient(glm::vec4(0.180f, 0.164f, 0.130f, 1.0f));
    g_matWoodHoney.setDiffuse(glm::vec4(0.720f, 0.656f, 0.520f, 1.0f));
    g_matWoodHoney.setSpecular(glm::vec4(0.30f, 0.30f, 0.30f, 1.0f));
    g_matWoodHoney.setShininess(24.0f);

    g_matWoodLightOak.setAmbient(glm::vec4(0.200f, 0.180f, 0.160f, 1.0f));
    g_matWoodLightOak.setDiffuse(glm::vec4(0.800f, 0.720f, 0.640f, 1.0f));
    g_matWoodLightOak.setSpecular(glm::vec4(0.30f, 0.30f, 0.30f, 1.0f));
    g_matWoodLightOak.setShininess(24.0f);

    g_matWoodBleached.setAmbient(glm::vec4(0.220f, 0.215f, 0.205f, 1.0f));
    g_matWoodBleached.setDiffuse(glm::vec4(0.880f, 0.860f, 0.820f, 1.0f));
    g_matWoodBleached.setSpecular(glm::vec4(0.30f, 0.30f, 0.30f, 1.0f));
    g_matWoodBleached.setShininess(24.0f);
}