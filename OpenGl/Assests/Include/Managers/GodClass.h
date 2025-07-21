#pragma once
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#include "baseShader.h"

class Camera;
class Object;
class Transform;
class RenderPipeline;

class GodClass
{
public:
	Camera* mainCamera;
	Object* mainLight;
	bool projectionDirty = true;

	void init(Camera* camera);
	static GodClass& getInstance()
	{
		static GodClass instance;
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
	void updateTime();

	RenderPipeline* getRenderPipeline();


	float getFrameTime()
	{
		return currentFrame - lastFrame;
	}

	baseShader* getFocusShader()
	{
		return focusShader.get();
	}

	void start();
	
	//update
	void run(GLFWwindow* window);

	
private:
	glm::mat4 projectionMat;

	unsigned int SCR_WIDTH = 800;
	unsigned int SCR_HEIGHT = 600;

	//time
	float currentFrame = 0;
	float lastFrame = 0;

	//focus shader
	std::unique_ptr<baseShader> focusShader;


	//RP
	RenderPipeline* renderPipeline = nullptr;



	GodClass() ;
	~GodClass();

	GodClass(GodClass&) = delete;
	GodClass& operator=(GodClass&) = delete;

	GodClass(GodClass&&) = delete;
	GodClass& operator=(GodClass&&) = delete;

};
