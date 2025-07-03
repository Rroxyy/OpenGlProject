#include "baseShader.h"
#include "ResourcePathManager.h"

baseShader::baseShader(const char* vertexPath, const char* fragmentPath)
	: Shader(vertexPath, fragmentPath)
{
	;
}

baseShader::baseShader()
	: baseShader(ResourcePathManager::getInstance().getBaseShaderVert().c_str(),
		ResourcePathManager::getInstance().getBaseShaderFrag().c_str())
{
}


