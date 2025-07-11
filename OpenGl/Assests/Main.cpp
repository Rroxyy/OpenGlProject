#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "camera.h"
#include "InputSystem.h"
#include "model.h"
#include "TextureResource.h"
#include "Shader.h"
#include "ResourcePathManager.h"
#include "UI_Manager.h"
#include "baseShader.h"
#include "girdMesh.h"
#include "globalParametersManager.h"
#include "Object.h"
#include "ObjectManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLFWwindow* glfwInitialize();
void imguiInitialize(GLFWwindow* window);
void ui_update();
// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    

    GLFWwindow* window = glfwInitialize();

    imguiInitialize(window);


    //ObjectManager::getInstance().loadJson();

    Object* grid = ObjectManager::getInstance().createObjectPtr("Grid");
    grid->AddComponent<gridMesh>();


    Object* tv = ObjectManager::getInstance().createObjectPtr("TV");
    Model tvModel("C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Resource/Mesh/tv/tv.obj");
    tv->AddComponent<Model>(std::move(tvModel));

    Object* plane = ObjectManager::getInstance().createObjectPtr("plane");

    Object* light = ObjectManager::getInstance().createObjectPtr("Light");
    globalParametersManager::getInstance().mainLight = light;





    //camera initialize
    globalParametersManager::getInstance().mainCamera = &camera;

    // build and compile shaders
    // -------------------------
    baseShader tvShader(ResourcePathManager::getInstance().getBaseShaderVert().c_str(),
        ResourcePathManager::getInstance().getBaseShaderFrag().c_str()
        );
    tvShader.setShaderName("tvShader");

    baseShader planeShader(ResourcePathManager::getInstance().getBaseShaderVert().c_str(),
        ResourcePathManager::getInstance().getBaseShaderFrag().c_str()
    );
    planeShader.setShaderName("planeShader");

    baseShader gridShader(
        ResourcePathManager::getInstance().getGridShaderVert().c_str(),
        ResourcePathManager::getInstance().getGridShaderFrag().c_str()
    );
    gridShader.setShaderName("gridShader");





    // load models
    // -----------

    

    Model planeModel("C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Resource/Mesh/plane.obj");

    TextureResource baseTex("C:\\Users\\Drwin\\Desktop\\render\\render3\\vs\\OpenGl\\Assests\\Resource\\Mesh\\tv\\tv_MatID.tga");
    TextureResource normalTex("C:\\Users\\Drwin\\Desktop\\render\\render3\\vs\\OpenGl\\Assests\\Resource\\Mesh\\tv\\tv_Normal_G.tga");



    tvShader.use();
    tvShader.useTexture("texture_base", baseTex);
    tvShader.useTexture("texture_normal", normalTex);

    //tvShader.PrintActiveUniforms();
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        //processInput(window);
        InputSystem::getInstance().checkInput(window, deltaTime);

        // render
        // ------
        //sky color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        //grid
        gridShader.use();
        gridShader.setMat4("model", model);
        grid->use();


        //plane
        planeShader.use();
        planeShader.setMat4("model", plane->GetComponent<Transform>()->getModelMat4());
        planeModel.Draw();


        // tv
        tvShader.use();
        baseTex.activeTexture();
        normalTex.activeTexture();

        // view/projection transformations
        tvShader.setMat4("model", tv->GetComponent<Transform>()->getModelMat4());
        tvShader.setVec3("cameraPos", camera.Position);

        tv->use();
        //tvModel.Draw();

        baseTex.deactivateTexture();
        normalTex.deactivateTexture();

        //ui
        ui_update();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void ui_update()
{
    static bool show_demo_window = false;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Control Panel");
    ImGui::Spacing();

    static float fps_accum = 0.0f;     // FPS 总和
    static int frame_count = 0;        // 帧计数器
    static float last_update = 0.0f;   // 上次更新时间
    static float avg_fps = 0.0f;       // 最终显示的平均 FPS

    float current_time = ImGui::GetTime();              // 当前时间（秒）
    fps_accum += ImGui::GetIO().Framerate;
    frame_count++;

    if (current_time - last_update >= 0.5f)             // 每隔 0.5 秒更新一次
    {
        avg_fps = fps_accum / frame_count;              // 平均 FPS
        fps_accum = 0.0f;
        frame_count = 0;
        last_update = current_time;
    }

    ImGui::Text("Avg FPS: %.1f", avg_fps);
    ImGui::Spacing();


    UIManager::getInstance().DrawAllUI();
   


    ImGui::Spacing();
    if (ImGui::Button("Show Demo Window"))
    {
        show_demo_window = !show_demo_window;
    }
    ImGui::End();


    if (show_demo_window)
    {
        ImGui::ShowDemoWindow(&show_demo_window);
        // 注意传引用，这样关闭 demo 窗口后变量也能自动变 false
    }


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLFWwindow* glfwInitialize()
{
    // glfw: initialize and configure
   // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(globalParametersManager::getInstance().getWidth(),
        globalParametersManager::getInstance().getHeight(), "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::abort();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::abort();
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    return window;
}

void imguiInitialize(GLFWwindow* window)
{
    //ui initialize and configure
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // 后端初始化
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    globalParametersManager::getInstance().setResolution(width, height);
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    InputSystem::getInstance().mouse_callback(window, xposIn, yposIn);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    InputSystem::getInstance().scroll_callback(window, xoffset, yoffset);

}



