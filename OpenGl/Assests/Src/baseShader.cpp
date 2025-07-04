#include "baseShader.h"
#include "ResourcePathManager.h"
#include "UI_Manager.h"

baseShader::baseShader(const char* vertexPath, const char* fragmentPath)
	: Shader(vertexPath, fragmentPath)
{
	UIManager::getInstance().Register([this]() {
		this->drawShaderUI();
		});
}

baseShader::baseShader()
	: baseShader(ResourcePathManager::getInstance().getBaseShaderVert().c_str(),
		ResourcePathManager::getInstance().getBaseShaderFrag().c_str())
{
	
}

baseShader::baseShader(std::string& _shaderName):
	baseShader(ResourcePathManager::getInstance().getBaseShaderVert().c_str(),
		ResourcePathManager::getInstance().getBaseShaderFrag().c_str())
{
	shaderName = _shaderName;
}

void baseShader::use() const
{
    Shader::use();
    update_shader_value();
}



void baseShader::drawShaderUI()
{
    if (ImGui::TreeNode(shaderName.c_str()))  // 顶层 Shader 名
    {
        if (ImGui::TreeNode("Lighting"))
        {
            ImGui::ColorEdit3("Light Color", (float*)&lightColor);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Normal Map"))
        {
            ImGui::Text("Use Normal Texture:");
            ImGui::RadioButton("On", &useNormalTex, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Off", &useNormalTex, 0);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Ambient"))
        {
            ImGui::SliderFloat("Strength", &ambientStrength, 0.0f, 1.0f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Specular"))
        {
            ImGui::SliderFloat("Strength", &specularStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Shininess", &shininess, 0.0f, 64.0f);
            ImGui::TreePop();
        }

        ImGui::TreePop();  // 收起顶层节点
    }
}



void baseShader::update_shader_value()const
{
    glm::vec3 shadercolor = glm::vec3(lightColor.x, lightColor.y, lightColor.z);
    setVec3("lightColor", shadercolor);

    setInt("useNormalTex", useNormalTex);
    setFloat("ambientStrength", ambientStrength);
    setFloat("specularStrength", specularStrength);
    setFloat("shininess", shininess);
}


void baseShader::setShaderName(std::string& _ShaderName)
{
    shaderName = _ShaderName;
}

void baseShader::setShaderName(std::string&& _ShaderName)
{
    shaderName = _ShaderName;
}


