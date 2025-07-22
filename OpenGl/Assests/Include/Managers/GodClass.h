#pragma once
#include <memory>
#include <glm/detail/type_mat.hpp>

#include "baseShader.h"
#include"camera.h"

//class Camera;
class Object;
class Transform;
class RenderPipeline;
struct GLFWwindow;



class GodClass
{
public:
	GLFWwindow* window = nullptr;

	//Camera* mainCamera;
	Object* mainLight;
	bool projectionDirty = true;

	void init(GLFWwindow* _window);
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

	Camera* getMainCamera() { return camera.get(); }

	void start();

	//call back
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


	//awake


	//update
	void run(GLFWwindow* window);

	
private:
	std::unique_ptr<Camera>camera;
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
