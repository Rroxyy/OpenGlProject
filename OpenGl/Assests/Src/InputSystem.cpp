#include "InputSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"

InputSystem::InputSystem()
{
    isMoving = false;
    firstMouse = true;
    camera =nullptr;
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

void InputSystem::setCamera(Camera* _camera)
{
    getInstance().camera = _camera;
}


void InputSystem::checkInput(GLFWwindow* window, float deltaTime)
{
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
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera->IsPushing = true;


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
}

void InputSystem::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!getInstance().isMoving)return;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}
void InputSystem::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!getInstance().isMoving)return;
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}