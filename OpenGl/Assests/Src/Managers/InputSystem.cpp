// InputSystem.cpp

#include <glad/glad.h>         // ✅ 必须第一个引入 OpenGL 的头，只用 glad 提供的

#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>

#include <imgui.h>
#include <imgui_internal.h>    // 推荐：有些 ImGuizmo 版本依赖这个
#include "imgui/ImGuizmo.h"

#include "InputSystem.h"
#include "camera.h"
#include "GodClass.h"


InputSystem::InputSystem()
{
    isMoving = false;
    firstMouse = true;
    operation = ImGuizmo::TRANSLATE;
}
InputSystem::~InputSystem()
{
    ;
}


InputSystem& InputSystem::getInstance()
{
	static InputSystem instance;
	return instance;
}




void InputSystem::checkInput(GLFWwindow* window)
{
    //exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (!isMoving)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        }

        isMoving = true;
    }
    else
    {
        if (isMoving)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        isMoving = false;
    }



    if (isMoving)
    {
        moveLogic(window);
    }
    else
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
	        
        }
        else
        {
            transformLogic(window);
        }
    }
}

void InputSystem::transformLogic(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        operation = ImGuizmo::TRANSLATE;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        operation = ImGuizmo::ROTATE;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        operation = ImGuizmo::SCALE;
}




void InputSystem::moveLogic(GLFWwindow* window)
{
    Camera* camera = GodClass::getInstance().getMainCamera();
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->IsPushing = true;

    float deltaTime = GodClass::getInstance().getFrameTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->ProcessKeyboard(UP, deltaTime);
    camera->IsPushing = false;
}





/// ///////////////////////////////////////////////////////////
void InputSystem::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    nowX = static_cast<float>(xposIn);
    nowY = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = nowX;
        lastY = nowY;
        firstMouse = false;
    }

    float xoffset = nowX - lastX;
    float yoffset = lastY - nowY; // reversed since y-coordinates go from bottom to top

    lastX = nowX;
    lastY = nowY;

    //std::cout << nowX << " " <<nowY << std::endl;

    if (!getInstance().isMoving)return;
    GodClass::getInstance().getMainCamera()->ProcessMouseMovement(xoffset, yoffset);
}
void InputSystem::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!getInstance().isMoving)return;
    GodClass::getInstance().getMainCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
}