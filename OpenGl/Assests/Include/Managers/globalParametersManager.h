#pragma once
#include <glm/gtc/matrix_transform.hpp>


class Camera;
class Object;

class Transform;

class globalParametersManager
{
public:
	static globalParametersManager& getInstance()
	{
		static globalParametersManager instance;
		return instance;
	}

	glm::mat4 getProjection();
	

	unsigned int getWidth() const { return SCR_WIDTH; }
	unsigned int getHeight() const { return SCR_HEIGHT; }

	void setResolution(unsigned int w, unsigned int h) {
		SCR_WIDTH = w;
		SCR_HEIGHT = h;
		projectionDirty = true; 
	}

	void updateCurrentFrame()
	{
		lastFrame = currentFrame;
		currentFrame = static_cast<float>(glfwGetTime());
	}

	float getFrameTime()
	{
		return currentFrame - lastFrame;
	}

	Camera* mainCamera;

	Object* mainLight;

	bool projectionDirty = true;


	
private:
	glm::mat4 projectionMat;

	unsigned int SCR_WIDTH = 800;
	unsigned int SCR_HEIGHT = 600;

	//time
	float currentFrame = 0;
	float lastFrame = 0;

	globalParametersManager() =default;
	~globalParametersManager() = default;

	globalParametersManager(globalParametersManager&) = delete;
	globalParametersManager& operator=(globalParametersManager&) = delete;

	globalParametersManager(globalParametersManager&&) = delete;
	globalParametersManager& operator=(globalParametersManager&&) = delete;

};
