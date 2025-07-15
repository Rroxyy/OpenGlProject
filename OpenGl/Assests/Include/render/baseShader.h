#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

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
	baseShader(const char* vertexPath, const char* fragmentPath, const std::string& _shaderName = "baseSHader");
	baseShader(const std::string& _shaderName);

	~baseShader()
	{

	}

	virtual void setShaderName(std::string& _ShaderName);
	virtual void setShaderName(std::string&& _ShaderName);
	virtual void blind_shader_value();

	//texture
	virtual void setTexture(const std::string& nameInShader,TextureResource* tr);
	virtual void blindTexturesChannel() const;
	virtual void unblindShaderValue() const;

	virtual void activeTexture() const;

	//component
	const std::string getComponentName() const override;
	nlohmann::json toJson() override;
	void loadJson(const nlohmann::json& js) override;
	std::unique_ptr<Component> clone() const override;
	void showUI() override;
	void start() override;


protected:
	std::string shaderName="baseShader";
	std::vector<std::pair<std::string, TextureResource*>>textureResourcesList;

	ImVec4 defaultColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);


	
};
