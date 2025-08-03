#include "Shaders/BaseLightShader.h"

#include "GodClass.h"
#include "Transform.h"
#include "camera.h"
#include "ShaderPathManager.h"
#include "TextureManager.h"
#include "Object/Object.h"

BaseLightShader::BaseLightShader() :baseShader(
    ShaderPathManager::getInstance().getBaseLightShaderVert().c_str(),
    ShaderPathManager::getInstance().getBaseLightShaderFrag().c_str(),
    "BaseLightShader")
{
    componentName = "BaseLightShader";
}

BaseLightShader::BaseLightShader(const std::string& shaderName) :baseShader(
    ShaderPathManager::getInstance().getBaseLightShaderVert().c_str(),
    ShaderPathManager::getInstance().getBaseLightShaderFrag().c_str(),
    shaderName)
{
    componentName = "BaseLightShader";
}


void BaseLightShader::showUI()
{
    if (ImGui::TreeNode(shaderName.c_str()))  // 顶层 Shader 名
    {
       

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



void BaseLightShader::use()
{
    baseShader::use();

    setVec3("cameraPos", GodClass::getInstance().getMainCamera()->Position);


    glm::vec3 temp = GodClass::getInstance().lightColor;

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


nlohmann::json BaseLightShader::toJson()
{
    nlohmann::json ret;
    nlohmann::json data;

    data["defaultColor"] = vec4ToJson(imToGlmVec4(defaultColor));
    data["useBaseTex"] = useBaseTex;
    data["useNormalTex"] = useNormalTex;
    data["ambientStrength"] = ambientStrength;
    data["specularStrength"] = specularStrength;
    data["shininess"] = shininess;

    nlohmann::json textures=nlohmann::json::array();
    for (auto& [name,tr]: textureResourcesList)
    {
        nlohmann::json item;
        item["nameInShader"] = name;
        item["textureResource"] = tr->toJson();
        textures.push_back(item);
    }

    data["textureResourcesList"] = textures;

    ret["componentName"] = getComponentName();
    ret["componentData"] = data;
    return ret;
}


void BaseLightShader::loadJson(const nlohmann::json& js)
{
    defaultColor = jsonToImVec4(js["defaultColor"]);
    useBaseTex = js["useBaseTex"].get<int>();
    useNormalTex = js["useNormalTex"].get<int>();
    ambientStrength = js["ambientStrength"].get<float>();
    specularStrength = js["specularStrength"].get<float>();
    shininess = js["shininess"].get<float>();


    auto textures = js["textureResourcesList"];
    for (auto& it:textures)
    {
        TextureResource* texturePtr = TextureManager::getInstance().getTextureResourceByJson(it["textureResource"]);
        std::string nameInShader = it["nameInShader"];
        setTexture(nameInShader, texturePtr);
    }

}




