#pragma once



struct GLFWwindow;

class InputSystem
{
public:
	bool isMoving;

	static InputSystem& getInstance();
	void checkInput(GLFWwindow* window);
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
	bool firstMouse;
};

