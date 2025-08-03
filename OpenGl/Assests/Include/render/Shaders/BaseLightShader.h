#pragma once

#include "baseShader.h"
class BaseLightShader:public baseShader
{
public :
	BaseLightShader();
	BaseLightShader(const std::string& shaderName);

	void showUI()override;
	void use() override;

	void loadJson(const nlohmann::json& js) override;
	nlohmann::json toJson() override;

private:
	int useBaseTex = 1;
	int useNormalTex = 1;
	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	float shininess = 32.0f;
};

