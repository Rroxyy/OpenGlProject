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
#include "Ray.h"
#include "Scene.h"


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
        SetMovingState(window);
    }
    else
    {
        UnsetMovingState(window);
    }

	moveLogic(window);
    idleStateLogic(window);
}

void InputSystem::SetMovingState(GLFWwindow* window)
{
    if (!isMoving)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse = true;
    }

    isMoving = true;
}
void InputSystem::UnsetMovingState(GLFWwindow* window)
{
    if (isMoving)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    isMoving = false;
}



void InputSystem::checkAABBLogic()
{
    auto& god = GodClass::getInstance();
    float w = static_cast<float>(god.getWidth());
    float h = static_cast<float>(god.getHeight());
    
    glm::mat4 P = god.getProjection();
    glm::mat4 V = god.getMainCamera()->GetViewMatrix();
    glm::vec3 rayOrigin = god.getMainCamera()->Position;

    // 1) 屏幕像素坐标 → NDC [-1,1]
    float x = 2.0f * nowX / w - 1.0f;
    float y = 1.0f - 2.0f * nowY / h;   // 反转 Y（窗口原点在左上时需要）

    glm::vec4 rayClip(x, y, -1.0f, 1.0f);
    glm::vec4 rayEye = glm::inverse(P) * rayClip;
    rayEye /= rayEye.w;

    glm::vec3 origin = GodClass::getInstance().getMainCamera()->Position;
    glm::vec3 rayDir = glm::normalize(glm::vec3(glm::inverse(V) * rayEye)-origin);

    Ray ray(origin, rayDir);

    Scene::getInstance().pickFocusObjectByRay(ray);
}


void InputSystem::ChangeOperateLogic(GLFWwindow* window)
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
    if (!isMoving)return;
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
void InputSystem::idleStateLogic(GLFWwindow* window)
{
    if (isMoving) return;

    bool leftDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool leftPressedThisFrame = leftDown && !LastFrameIsLeftPress;
    bool leftReleasedThisFrame = !leftDown && LastFrameIsLeftPress;

    // 建议在本帧已经调用过 ImGuizmo::Manipulate/SetRect 之后再查询
    bool gizmoOver = ImGuizmo::IsOver();   // 鼠标是否悬停在 Gizmo 上（手柄/轴等）
    bool gizmoUsing = ImGuizmo::IsUsing();  // 是否正在拖拽 Gizmo

    // ImGui UI 捕获（面板、按钮等）
    bool uiWantsMouse = ImGui::GetIO().WantCaptureMouse;

    // 若按下发生在 Gizmo 上或被 UI 捕获，则本次点击不做射线拾取
    if (leftPressedThisFrame && !gizmoOver && !uiWantsMouse)
    {
        std::cout << "Ray picking\n";
        checkAABBLogic();
    }

    LastFrameIsLeftPress = leftDown;
    ChangeOperateLogic(window);
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

    if (!getInstance().isMoving)return;
    GodClass::getInstance().getMainCamera()->ProcessMouseMovement(xoffset, yoffset);
}
void InputSystem::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!getInstance().isMoving)return;
    GodClass::getInstance().getMainCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
}