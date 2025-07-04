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
protected:
	std::string shaderName="baseShader";


	ImVec4 lightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);;
	int useNormalTex = 0;
	float ambientStrength = 0.1;


	float specularStrength = 0.5;
	float shininess = 32.0;


private:
};
