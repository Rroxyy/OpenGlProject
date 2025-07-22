#include <glad/glad.h>       
#include <GLFW/glfw3.h>      
#include "GodClass.h"

#include "baseShader.h"
#include "camera.h"
#include "RenderPipeline.h"
#include "ResourcePathManager.h"
#include "Scene.h"
#include "UI_Manager.h"

GodClass::GodClass()
{
   
}

GodClass::~GodClass()
{
	
}


void GodClass::init(GLFWwindow* _window)
{

   
    window = _window;
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

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

	UIManager::getInstance().DrawAllUI();

}




glm::mat4  GodClass::getProjection()
{
	if (projectionDirty)
	{
		projectionMat = glm::perspective(glm::radians(GodClass::getMainCamera()->Zoom),
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





void GodClass::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    GodClass::getInstance().setResolution(width, height);
    glViewport(0, 0, width, height);
}

void GodClass::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    InputSystem::getInstance().mouse_callback(window, xposIn, yposIn);
}

void GodClass::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    InputSystem::getInstance().scroll_callback(window, xoffset, yoffset);
}