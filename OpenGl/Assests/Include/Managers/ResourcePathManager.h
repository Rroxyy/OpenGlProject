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

	const std::string& getBaseLightShaderVert()const
	{
		return baseLightShaderVert;
	}
	const std::string& getBaseLightShaderFrag()const
	{
		return baseLightShaderFrag;
	}

	
private:
	const std::string baseShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/BaseShader/baseShader.vert";
	const std::string baseShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/BaseShader/baseShader.frag";

	const std::string baseLightShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/BaseLightShader/baseLightShader.vert";
	const std::string baseLightShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/BaseLightShader/baseLightShader.frag";

	const std::string gridShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/grid/grid.vert";
	const std::string gridShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/grid/grid.frag";

	ResourcePathManager()=default;
	~ResourcePathManager()=default;

	ResourcePathManager(ResourcePathManager&) = delete;
	ResourcePathManager(ResourcePathManager&&) = delete;

	ResourcePathManager& operator=(ResourcePathManager&) = delete;
	ResourcePathManager& operator=(ResourcePathManager&&) = delete;
};
