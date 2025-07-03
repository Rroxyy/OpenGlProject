#pragma once
#include <string>


class ResourcePathManager
{
public:
	static ResourcePathManager& getInstance();
	const std::string& getBaseShaderVert()const;
	const std::string& getBaseShaderFrag()const;


private:
	const std::string baseShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/model_load.vert";
	const std::string baseShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/model_load.frag";



	ResourcePathManager()=default;
	~ResourcePathManager()=default;

	ResourcePathManager(ResourcePathManager&) = delete;
	ResourcePathManager(ResourcePathManager&&) = delete;

	ResourcePathManager& operator=(ResourcePathManager&) = delete;
	ResourcePathManager& operator=(ResourcePathManager&&) = delete;
};
