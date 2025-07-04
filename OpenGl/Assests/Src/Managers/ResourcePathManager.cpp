#include "ResourcePathManager.h"


ResourcePathManager& ResourcePathManager::getInstance()
{
	static ResourcePathManager instance;
	return instance;
}

const std::string& ResourcePathManager::getBaseShaderVert() const
{
	return baseShaderVert;
}

const std::string& ResourcePathManager::getBaseShaderFrag() const
{
	return baseShaderFrag;
}
