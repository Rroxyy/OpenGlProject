#include "baseShader.h"

#include "camera.h"
#include "globalParametersManager.h"
#include "Object.h"
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


void baseShader::PrintActiveUniforms()
{
    GLint count;
    glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &count);

    char name[128];
    for (int i = 0; i < count; ++i) {
        GLenum type;
        GLint size;
        glGetActiveUniform(ID, i, sizeof(name), nullptr, &size, &type, name);

        GLint location = glGetUniformLocation(ID, name);

        std::cout << "Uniform #" << i << ": " << name << " | type: " << type << " | location: " << location << std::endl;
    }
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

        if (ImGui::TreeNode("BaseColor"))
        {
            ImGui::Text("Use Base Texture:");
            ImGui::RadioButton("On", &useBaseTex, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Off", &useBaseTex, 0);
            ImGui::Spacing();

            ImGui::ColorEdit3("Light Color", (float*)&defualtColor);
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
    setMat4("projection", globalParametersManager::getInstance().getProjection());
    setMat4("view", globalParametersManager::getInstance().mainCamera->GetViewMatrix());

    setVec3("cameraPos", globalParametersManager::getInstance().mainCamera->Position);


    glm::vec3 temp = glm::vec3(lightColor.x, lightColor.y, lightColor.z);
    setVec3("lightColor", temp);
    setVec3("lightDir",globalParametersManager::getInstance().mainLight->GetComponent<Transform>()->getBackDir());

    setInt("useBaseTex", useBaseTex);
    temp = glm::vec3(defualtColor.x, defualtColor.y, defualtColor.z);
    setVec3("defaultColor", temp);

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


