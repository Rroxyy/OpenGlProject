#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <imgui.h>
#include <sstream>
#include <iostream>

#include "Component.h"
#include "EnumClass.h"
#include "Shader.h"

class RenderContext;

class baseShader : public Shader,public Component
{
public:
	//render
	bool useDepth = true;
	CullMode cullMode = CullMode::Back;

	baseShader();
	baseShader(const char* vertexPath, const char* fragmentPath, const std::string& _shaderName = "baseShader");
	baseShader(const std::string& _shaderName);

	~baseShader()
	{

	}

	virtual void setShaderName(const std::string& _ShaderName);
	virtual const std::string& getShaderName();
	virtual void use();
	virtual void use(Object* obj);
	virtual void unuse() const;

	//texture
	virtual void setTexture(const std::string& nameInShader,TextureResource* tr);


	//component
	const std::string getComponentName() const override;
	nlohmann::json toJson() override;
	void loadJson(const nlohmann::json& js) override;
	std::unique_ptr<Component> clone() const override;
	void showUI() override;
	void start() override;

	//render
	virtual void setRenderState(bool _useDepth=true,CullMode _cullMode=CullMode::Front)
	{
		useDepth = _useDepth;
		cullMode = _cullMode;
	}
	

protected:
	std::string shaderName="baseShader";
	std::vector<std::pair<std::string, TextureResource*>>textureResourcesList;

	

	ImVec4 defaultColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	virtual void blindTexturesChannel() const;
	virtual void activeTexture() const;


	
};
