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
#include "ModelComponent.h"
#include "TextureResource.h"
#include "Shader.h"
#include "ShaderPathManager.h"
#include "UI_Manager.h"
#include "girdMesh.h"
#include "GodClass.h"
#include "Object.h"
#include "Scene.h"
#include "Render.h"
#include "TextureManager.h"
#include "imgui/ImGuizmo.h"
#include "Shaders/BaseLightShader.h"



GLFWwindow* glfwInitialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(GodClass::getInstance().getWidth(),
        GodClass::getInstance().getHeight(), "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::abort();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, GodClass::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, GodClass::mouse_callback);
    glfwSetScrollCallback(window, GodClass::scroll_callback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::abort();
    }
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    std::cout << "glfwInitialize ok" << std::endl;
    return window;
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

    std::cout << "imguiInitialize ok" << std::endl;
}
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    GLFWwindow* window = glfwInitialize();
    imguiInitialize(window);
    GodClass::getInstance().init(window);

    //Scene::getInstance().loadJson();

    Object* grid = Scene::getInstance().createObjectPtr("Grid");
    grid->AddComponent<ModelComponent>();


    auto grid_MR = std::make_shared<ModelResource>();
    grid_MR->moveMesh(new gridMesh());
    auto gridModel=ModelManager::getInstance().registerModel("gridModel", std::move(grid_MR));

    grid->GetComponentAs<ModelComponent>()->resetModel(gridModel);

    baseShader gridShader("gridShader");
    grid->AddComponent<baseShader>(gridShader);
    grid->AddComponent<Render>();


    ///////////////////////////////////////////////
    //tv
    Object* tv = Scene::getInstance().createObjectPtr("TV");
    auto tvModelResource = ModelManager::getInstance().loadModel("C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Resource/Mesh/tv/tv.obj");
    tv->AddComponent<ModelComponent>(std::move(tvModelResource));
    BaseLightShader tvShader("tvShader");
    tv->AddComponent<BaseLightShader>(std::move(tvShader));
    tv->AddComponent<Render>();


    TextureResource* baseTex = TextureManager::getInstance()
	.createTextureResourceByPath("C:\\Users\\Drwin\\Desktop\\render\\render3\\vs\\OpenGl\\Assests\\Resource\\Mesh\\tv\\tv_MatID.tga");
	TextureResource * normalTex = TextureManager::getInstance()
	.createTextureResourceByPath("C:\\Users\\Drwin\\Desktop\\render\\render3\\vs\\OpenGl\\Assests\\Resource\\Mesh\\tv\\tv_Normal_G.tga");

    tv->GetComponentAs<BaseLightShader>()->setTexture("texture_base", baseTex);
    tv->GetComponentAs<BaseLightShader>()->setTexture("texture_normal",normalTex);



    Object* plane = Scene::getInstance().createObjectPtr("plane");
    auto planeModelResource = ModelManager::getInstance().loadModel("C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Resource/Mesh/plane.obj");
    plane->AddComponent<ModelComponent>(std::move(planeModelResource));
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
    std::cout << "exit" << std::endl;
    return 0;
}


