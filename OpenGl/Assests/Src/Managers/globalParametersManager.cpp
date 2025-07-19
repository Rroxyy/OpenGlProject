#include <glad/glad.h>       
#include <GLFW/glfw3.h>      
#include "globalParametersManager.h"


#include "camera.h"
glm::mat4  globalParametersManager::getProjection()
{
	if (projectionDirty)
	{
		projectionMat = glm::perspective(glm::radians(mainCamera->Zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		projectionDirty = false;
	}
	return projectionMat;
}

void globalParametersManager::updateCurrentFrame()
{
	lastFrame = currentFrame;
	currentFrame = static_cast<float>(glfwGetTime());
}