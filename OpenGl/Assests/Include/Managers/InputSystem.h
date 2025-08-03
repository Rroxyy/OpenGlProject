#pragma once
#include <imgui.h>
#include "imgui/ImGuizmo.h"


struct GLFWwindow;
class InputSystem
{
public:
	bool isMoving;
	ImGuizmo::OPERATION operation;


	static InputSystem& getInstance();
	void idleStateLogic(GLFWwindow* window);
	void SetMovingState(GLFWwindow* window);
	void UnsetMovingState(GLFWwindow* window);


	void checkInput(GLFWwindow* window);
	void moveLogic(GLFWwindow* window);
	void ChangeOperateLogic(GLFWwindow* window);
	void checkAABBLogic();

	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


	// 禁用拷贝构造和拷贝赋值
	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

	// 禁用移动构造和移动赋值
	InputSystem(InputSystem&&) = delete;
	InputSystem& operator=(InputSystem&&) = delete;
private:
	InputSystem();
	~InputSystem();

	float lastX;
	float lastY;

	float nowX;
	float nowY;
	bool firstMouse;

	float LastClickTime = -1000.0f;
	bool LastFrameIsLeftPress = false;
};

