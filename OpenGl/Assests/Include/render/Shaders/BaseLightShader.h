#pragma once

#include "baseShader.h"
class BaseLightShader:public baseShader
{
public :
	BaseLightShader();
	BaseLightShader(const std::string& shaderName);

	void showUI()override;
	void blind_shader_value() override;

private:
	int useBaseTex = 1;
	int useNormalTex = 1;

	ImVec4 lightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);


	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	float shininess = 32.0f;
};

