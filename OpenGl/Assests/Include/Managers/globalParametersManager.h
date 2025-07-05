#pragma once

class Camera;
class globalParametersManager
{
public:
	static globalParametersManager& getInstance()
	{
		static globalParametersManager instance;
		return instance;
	}

	glm::mat4 getProjection()
	{
		if (projectionDirty)
		{
			projectionMat = glm::perspective(glm::radians(mainCamera->Zoom),
				(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			projectionDirty = false;
		}
		return projectionMat;
	}

	unsigned int getWidth() const { return SCR_WIDTH; }
	unsigned int getHeight() const { return SCR_HEIGHT; }

	void setResolution(unsigned int w, unsigned int h) {
		SCR_WIDTH = w;
		SCR_HEIGHT = h;
		projectionDirty = true; 
	}

	Camera* mainCamera;

	bool projectionDirty = true;
private:
	glm::mat4 projectionMat;

	unsigned int SCR_WIDTH = 800;
	unsigned int SCR_HEIGHT = 600;
	globalParametersManager() =default;
	~globalParametersManager() = default;

	globalParametersManager(globalParametersManager&) = delete;
	globalParametersManager& operator=(globalParametersManager&) = delete;

	globalParametersManager(globalParametersManager&&) = delete;
	globalParametersManager& operator=(globalParametersManager&&) = delete;

};
