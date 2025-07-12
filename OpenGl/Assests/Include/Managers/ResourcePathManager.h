#pragma once
#include <string>


class ResourcePathManager
{
public:
	static ResourcePathManager& getInstance();
	const std::string& getBaseShaderVert()const;
	const std::string& getBaseShaderFrag()const;

	const std::string& getGridShaderVert()const
	{
		return gridShaderVert;
	}
	const std::string& getGridShaderFrag()const
	{
		return gridShaderFrag;
	}

	
private:
	const std::string baseShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/baseShader.vert";
	const std::string baseShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/baseShader.frag";

	const std::string gridShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/grid/grid.vert";
	const std::string gridShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/grid/grid.frag";

	ResourcePathManager()=default;
	~ResourcePathManager()=default;

	ResourcePathManager(ResourcePathManager&) = delete;
	ResourcePathManager(ResourcePathManager&&) = delete;

	ResourcePathManager& operator=(ResourcePathManager&) = delete;
	ResourcePathManager& operator=(ResourcePathManager&&) = delete;
};
