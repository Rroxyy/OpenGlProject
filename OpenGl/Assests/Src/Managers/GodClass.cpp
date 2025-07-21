#include <glad/glad.h>       
#include <GLFW/glfw3.h>      
#include "GodClass.h"

#include "baseShader.h"
#include "camera.h"
#include "RenderPipeline.h"
#include "ResourcePathManager.h"
#include "Scene.h"

GodClass::GodClass()
{
	
}

GodClass::~GodClass()
{
	
}


void GodClass::init(Camera* camera)
{
	mainCamera = camera;
	focusShader = std::make_unique<baseShader>(
		ResourcePathManager::getInstance().getFocusShaderVert().c_str(),
		ResourcePathManager::getInstance().getFocusShaderFrag().c_str(),
		"focusShader"
	);
	renderPipeline = &RenderPipeline::getInstance();
}


void GodClass::start()
{
	Scene::getInstance().start();
}


void GodClass::run(GLFWwindow* window)
{

	updateTime();

	InputSystem::getInstance().checkInput(window);



	Scene::getInstance().beforeUpdate();
	Scene::getInstance().update();
	Scene::getInstance().afterUpdate();


	renderPipeline->renderScene();



}




glm::mat4  GodClass::getProjection()
{
	if (projectionDirty)
	{
		projectionMat = glm::perspective(glm::radians(mainCamera->Zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		projectionDirty = false;
	}
	return projectionMat;
}

void GodClass::updateTime()
{
	lastFrame = currentFrame;
	currentFrame = static_cast<float>(glfwGetTime());
}

RenderPipeline* GodClass::getRenderPipeline()
{
	return renderPipeline;
}
