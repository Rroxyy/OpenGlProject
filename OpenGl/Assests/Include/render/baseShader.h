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
	baseShader(const char* vertexPath, const char* fragmentPath);
	baseShader(std::string&& _shaderName);

	~baseShader()
	{

	}

	virtual void setShaderName(std::string& _ShaderName);
	virtual void setShaderName(std::string&& _ShaderName);
	virtual void blind_shader_value();
	virtual void PrintActiveUniforms();

	//texture
	void setTexture(const std::string& nameInShader,TextureResource* tr);
	void blindTexturesChannel() const;
	void unblindTexturesChannel() const;

	void activeTexture() const;

	//component
	const std::string getComponentName() const override;
	nlohmann::json toJson() override;
	void loadJson(const nlohmann::json& js) override;
	std::unique_ptr<Component> clone() const override;
	void showUI() override;

	void start() override;
	//void beforeUpdate() override;
	//void update()override;

protected:
	std::string shaderName="baseShader";
	std::vector<std::pair<std::string, TextureResource*>>textureResourcesList;
private:
	ImVec4 defaultColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	
};
