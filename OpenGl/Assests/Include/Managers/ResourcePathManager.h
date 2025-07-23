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

	const std::string& getOutlineShaderVert()const
	{
		return outlineShaderVert;
	}
	const std::string& getOutlineShaderFrag()const
	{
		return outlineShaderFrag;
	}


	const std::string& getQuadOutlineShaderVert()const
	{
		return QuadOutlineShaderVert;
	}
	const std::string& getQuadOutlineShaderFrag()const
	{
		return QuadOutlineShaderFrag;
	}
	
private:
	const std::string baseShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/BaseShader/baseShader.vert";
	const std::string baseShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/BaseShader/baseShader.frag";

	const std::string baseLightShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/BaseLightShader/baseLightShader.vert";
	const std::string baseLightShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/BaseLightShader/baseLightShader.frag";

	const std::string outlineShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/Outline/Outline.vert";
	const std::string outlineShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/Outline/Outline.frag";

	const std::string QuadOutlineShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/Outline/QuadOutline.vert";
	const std::string QuadOutlineShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/Outline/QuadOutline.frag";



	const std::string gridShaderVert = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/grid/grid.vert";
	const std::string gridShaderFrag = "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/grid/grid.frag";

	

	ResourcePathManager()=default;
	~ResourcePathManager()=default;

	ResourcePathManager(ResourcePathManager&) = delete;
	ResourcePathManager(ResourcePathManager&&) = delete;

	ResourcePathManager& operator=(ResourcePathManager&) = delete;
	ResourcePathManager& operator=(ResourcePathManager&&) = delete;
};
