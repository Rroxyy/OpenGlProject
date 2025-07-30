#include "ShaderPathManager.h"


ShaderPathManager& ShaderPathManager::getInstance()
{
	static ShaderPathManager instance;
	return instance;
}

const std::string& ShaderPathManager::getBaseShaderVert() const
{
	return baseShaderVert;
}

const std::string& ShaderPathManager::getBaseShaderFrag() const
{
	return baseShaderFrag;
}
