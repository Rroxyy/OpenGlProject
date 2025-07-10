#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <imgui.h>
#include <sstream>
#include <iostream>

#include "Shader.h"

class baseShader : public Shader
{
public:
	baseShader();
	baseShader(const char* vertexPath, const char* fragmentPath);
	baseShader(std::string& _shaderName);

	void setShaderName(std::string& _ShaderName);
	void setShaderName(std::string&& _ShaderName);
	void drawShaderUI();
	void update_shader_value()const;
	void use()const override;
	void PrintActiveUniforms();
protected:
	std::string shaderName="baseShader";
	int useBaseTex = 1;
	int useNormalTex = 1;

	ImVec4 lightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	ImVec4 defualtColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	float shininess = 32.0f;


private:
};
