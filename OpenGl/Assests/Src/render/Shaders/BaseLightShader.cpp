#include "Shaders/BaseLightShader.h"

#include "GodClass.h"
#include "Transform.h"
#include "camera.h"
#include "ResourcePathManager.h"
#include "Object/Object.h"

BaseLightShader::BaseLightShader() :baseShader(
    ResourcePathManager::getInstance().getBaseLightShaderVert().c_str(),
    ResourcePathManager::getInstance().getBaseLightShaderFrag().c_str(),
    "BaseLightShader")
{
}

BaseLightShader::BaseLightShader(const std::string& shaderName) :baseShader(
    ResourcePathManager::getInstance().getBaseLightShaderVert().c_str(),
    ResourcePathManager::getInstance().getBaseLightShaderFrag().c_str(),
    shaderName)
{
}


void BaseLightShader::showUI()
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

            if (!useBaseTex)
            {
                ImGui::ColorEdit3("Default Color", (float*)&defaultColor);
            }
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



void BaseLightShader::use(RenderContext& context)
{
    baseShader::use(context);



    setVec3("cameraPos", GodClass::getInstance().mainCamera->Position);


    glm::vec3 temp = glm::vec3(lightColor.x, lightColor.y, lightColor.z);
    setVec3("lightColor", temp);
    setVec3("lightDir", GodClass::getInstance().mainLight->GetComponentAs<Transform>()->getBackDir());

    setInt("useBaseTex", useBaseTex);
   
    temp = glm::vec3(defaultColor.x, defaultColor.y, defaultColor.z);
    setVec3("defaultColor", temp);
    
    

    setInt("useNormalTex", useNormalTex);
    setFloat("ambientStrength", ambientStrength);
    setFloat("specularStrength", specularStrength);
    setFloat("shininess", shininess);
}

