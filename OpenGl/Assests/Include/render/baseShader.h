#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <imgui.h>
#include <sstream>
#include <iostream>

#include "Component.h"
#include "Shader.h"

class baseShader : public Shader,public Component
{
public:
	baseShader();
	baseShader(const char* vertexPath, const char* fragmentPath);
	baseShader(std::string&& _shaderName);
	~baseShader()
	{
		std::cout << "Class baseShader:               " << "Release component: " << componentName << std::endl;

	}

	virtual void setShaderName(std::string& _ShaderName);
	virtual void setShaderName(std::string&& _ShaderName);
	virtual void update_shader_value();
	virtual void PrintActiveUniforms();

	//component
	const std::string getComponentName() const override;
	nlohmann::json toJson() override;
	void loadJson(const nlohmann::json& js) override;
	std::unique_ptr<Component> clone() const override;
	void showUI() override;
	void update()override;

protected:
	std::string shaderName="baseShader";

private:
	ImVec4 defaultColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	
};
