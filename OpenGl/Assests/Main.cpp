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
#include "girdMesh.h"
#include "GodClass.h"
#include "Object.h"
#include "Scene.h"
#include "Render.h"
#include "TextureManager.h"
#include "imgui/ImGuizmo.h"
#include "Shaders/BaseLightShader.h"

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



glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    GLFWwindow* window = glfwInitialize();
    imguiInitialize(window);

    GodClass::getInstance().init(&camera);

    //Scene::getInstance().loadJson();

    Object* grid = Scene::getInstance().createObjectPtr("Grid");
    grid->AddComponent<Model>();


    gridMesh grid_mesh;

    grid->GetComponentAs<Model>()->addMesh(&grid_mesh);

    baseShader gridShader("gridShader");
    grid->AddComponent<baseShader>(gridShader);
    grid->AddComponent<Render>();


    ///////////////////////////////////////////////
    //tv
    Object* tv = Scene::getInstance().createObjectPtr("TV");
    Model tvModel("C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Resource/Mesh/tv/tv.obj");
    tv->AddComponent<Model>(std::move(tvModel));
    BaseLightShader tvShader("tvShader");
    baseShader tempShader(ResourcePathManager::getInstance().getFocusShaderVert().c_str(), ResourcePathManager::getInstance().getFocusShaderFrag().c_str());
    tv->AddComponent<baseShader>(std::move(tempShader));
    //tv->AddComponent<BaseLightShader>(std::move(tvShader));
    tv->AddComponent<Render>();


  /*  TextureResource* baseTex = TextureManager::getInstance()
	.createTextureResource("C:\\Users\\Drwin\\Desktop\\render\\render3\\vs\\OpenGl\\Assests\\Resource\\Mesh\\tv\\tv_MatID.tga");
	TextureResource * normalTex = TextureManager::getInstance()
	.createTextureResource("C:\\Users\\Drwin\\Desktop\\render\\render3\\vs\\OpenGl\\Assests\\Resource\\Mesh\\tv\\tv_Normal_G.tga");

    tv->GetComponentAs<BaseLightShader>()->setTexture("texture_base", baseTex);
    tv->GetComponentAs<BaseLightShader>()->setTexture("texture_normal",normalTex);*/



    Object* plane = Scene::getInstance().createObjectPtr("plane");
    Model planeModel("C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Resource/Mesh/plane.obj");
    plane->AddComponent<Model>(std::move(planeModel));
    baseShader planeShader("C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/DepthShader/baseShader.vert",
        "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/DepthShader/baseShader.frag",
        "planeShader");
    plane->AddComponent<baseShader>(std::move(planeShader));
    plane->AddComponent<Render>();


    Object* light = Scene::getInstance().createObjectPtr("Light");
    GodClass::getInstance().mainLight = light;


    //camera initialize


    



    

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //start
    GodClass::getInstance().start();

    while (!glfwWindowShouldClose(window))
    {
        GodClass::getInstance().run(window);

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
    std::cout << "ok" << std::endl;
    return 0;
}

void ui_update()
{
    static bool show_demo_window = false;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();


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


    if (ImGui::TreeNode("Gizmos test"))
    {
        ImGuizmo::BeginFrame();

        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);  // 设置操作区域

        static glm::mat4 modelMatrix = glm::mat4(1.0f); // static 防止每帧重置

        ImGuizmo::SetOrthographic(false); 

        ImGuizmo::Manipulate(
            glm::value_ptr(GodClass::getInstance().mainCamera->GetViewMatrix()),
            glm::value_ptr(GodClass::getInstance().getProjection()),
            ImGuizmo::TRANSLATE,
            ImGuizmo::WORLD,
            glm::value_ptr(modelMatrix));

        ImGui::TreePop();
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
void imguiInitialize(GLFWwindow* window)
{
    //ui initialize and configure
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.FontGlobalScale = 1.5f;

    ImGui::StyleColorsDark();

    // 后端初始化
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
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
    GLFWwindow* window = glfwCreateWindow(GodClass::getInstance().getWidth(),
        GodClass::getInstance().getHeight(), "LearnOpenGL", NULL, NULL);
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


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    GodClass::getInstance().setResolution(width, height);
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



