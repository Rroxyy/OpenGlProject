#pragma once

#include "baseShader.h"
class BaseLightShader:public baseShader
{
public :
	BaseLightShader();
	BaseLightShader(std::string& _shaderName);

	void setShaderName(std::string& _ShaderName)override;
	void setShaderName(std::string&& _ShaderName)override;
	void showUI()override;
	void update_shader_value() override;
	void use() override;
	virtual void PrintActiveUniforms() override;

private:
	int useBaseTex = 1;
	int useNormalTex = 1;

	ImVec4 lightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	ImVec4 defaultColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	float shininess = 32.0f;
};

