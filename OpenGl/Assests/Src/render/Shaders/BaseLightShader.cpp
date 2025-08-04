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
        // --- Base Color ---
        if (ImGui::TreeNode("BaseColor"))
        {
            ImGui::Text("Use Base Texture:");
            ImGui::RadioButton("On##BaseTex", &useBaseTex, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Off##BaseTex", &useBaseTex, 0);
            ImGui::Spacing();

            if (!useBaseTex)
            {
                ImGui::ColorEdit3("Default Color", (float*)&defaultColor);
            }
            ImGui::TreePop();
        }

        // --- Normal Map ---
        if (ImGui::TreeNode("Normal Map"))
        {
            ImGui::Text("Use Normal Texture:");
            ImGui::RadioButton("On##NormalTex", &useNormalTex, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Off##NormalTex", &useNormalTex, 0);
            ImGui::TreePop();
        }

        // --- Specular Map (Spec_Gloss) ---
        if (ImGui::TreeNode("Specular Map"))
        {
            ImGui::Text("Use SpecGloss Texture:");
            ImGui::RadioButton("On##SpecGlossTex", &useSpecGlossTex, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Off##SpecGlossTex", &useSpecGlossTex, 0);

            if (!useSpecGlossTex)
            {
                ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f);
                ImGui::SliderFloat("Shininess", &shininess, 1.0f, 256.0f);
            }

            ImGui::TreePop();
        }

        // --- Displacement Map ---
        if (ImGui::TreeNode("Displacement Map"))
        {
            ImGui::Text("Use Displacement Texture:");
            ImGui::RadioButton("On##DispTex", &useDispTex, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Off##DispTex", &useDispTex, 0);

            if (useDispTex)
            {
                ImGui::SliderFloat("Displacement Scale", &dispTexScale, 0.0f, 0.1f);
            }

            ImGui::TreePop();
        }

        // --- Ambient ---
        if (ImGui::TreeNode("Ambient"))
        {
            ImGui::SliderFloat("Strength", &ambientStrength, 0.0f, 1.0f);
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
    setVec3("lightDir", GodClass::getInstance().mainLight->GetComponentAs<Transform>()->getBackDir());//light模型导入的时候方向放反了，以后记得change

    setInt("useBaseTex", useBaseTex);
    setInt("useNormalTex", useNormalTex);
    setInt("useSpecGlossTex", useSpecGlossTex);
    setInt("useDispTex", useDispTex);

    setVec3("defaultColor", glm::vec3(defaultColor.x, defaultColor.y, defaultColor.z));

    setFloat("ambientStrength", ambientStrength);
    setFloat("specularStrength", specularStrength);
    setFloat("shininess", shininess);
    setFloat("dispTexScale", dispTexScale);
}



nlohmann::json BaseLightShader::toJson()
{
    nlohmann::json ret;
    nlohmann::json data;

    data["defaultColor"] = vec4ToJson(imToGlmVec4(defaultColor));
    data["useBaseTex"] = useBaseTex;
    data["useNormalTex"] = useNormalTex;
    data["useSpecGlossTex"] = useSpecGlossTex;
    data["useDispTex"] = useDispTex;

    data["dispTexScale"] = dispTexScale;

    data["ambientStrength"] = ambientStrength;
    data["specularStrength"] = specularStrength;
    data["shininess"] = shininess;

    nlohmann::json textures = nlohmann::json::array();
    for (auto& [name, tr] : textureResourcesList)
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
    useBaseTex = js.value("useBaseTex", useBaseTex);
    useNormalTex = js.value("useNormalTex", useNormalTex);
    useSpecGlossTex = js.value("useSpecGlossTex", useSpecGlossTex);
    useDispTex = js.value("useDispTex", useDispTex);

    dispTexScale = js.value("dispTexScale", dispTexScale);

    ambientStrength = js.value("ambientStrength", ambientStrength);
    specularStrength = js.value("specularStrength", specularStrength);
    shininess = js.value("shininess", shininess);

    if (js.contains("textureResourcesList"))
    {
        auto textures = js["textureResourcesList"];
        for (auto& it : textures)
        {
            TextureResource* texturePtr = TextureManager::getInstance().getTextureResourceByJson(it["textureResource"]);
            std::string nameInShader = it["nameInShader"];
            setTexture(nameInShader, texturePtr);
        }
    }
}





