#include "baseShader.h"

#include "camera.h"
#include "globalParametersManager.h"
#include "Object.h"
#include "ResourcePathManager.h"
#include "UI_Manager.h"
#include "plugins.h"
#include "TextureResource.h"

baseShader::baseShader(const char* vertexPath, const char* fragmentPath,const std::string& _shaderName)
	: Shader(vertexPath, fragmentPath)
{
    componentName = "Shader";
    shaderName = _shaderName;
}

baseShader::baseShader()
	: baseShader(ResourcePathManager::getInstance().getBaseShaderVert().c_str(),
		ResourcePathManager::getInstance().getBaseShaderFrag().c_str())
{
	
}

baseShader::baseShader(const std::string& _shaderName): baseShader(ResourcePathManager::getInstance().getBaseShaderVert().c_str(),
    ResourcePathManager::getInstance().getBaseShaderFrag().c_str(), _shaderName)
{
	
}





void baseShader::start()
{
    Shader::use();
    activeTexture();
}



void baseShader::activeTexture()const
{
    int textureChannel = 0;
    for (const auto& it : textureResourcesList)
    {
        blindTextureToShader(it.first, textureChannel);
        textureChannel++;
    }
}

void baseShader::blindTexturesChannel()const
{
    int textureChannel = 0;
    for (const auto& it : textureResourcesList)
    {
        GLenum glChannel = static_cast<GLenum>(GL_TEXTURE0 + textureChannel);
        glActiveTexture(glChannel);
        glBindTexture(GL_TEXTURE_2D, it.second->getGLtexture_id());
        textureChannel++;
    }

}

void baseShader::unblindShaderValue()const
{
    int textureChannel = 0;
    for (const auto& it : textureResourcesList)
    {
        GLenum glChannel = static_cast<GLenum>(GL_TEXTURE0 + textureChannel);
        glActiveTexture(glChannel);
        glBindTexture(GL_TEXTURE_2D, 0);
        textureChannel++;
    }
}


void baseShader::setTexture(const std::string& nameInShader, TextureResource* tr)
{
	if (textureResourcesList.size()>32)
	{
        std::cout << "What the hell r u doing? Stop it!" << std::endl;
        std::abort();
	}
    textureResourcesList.emplace_back(nameInShader, tr); 
}







void baseShader::showUI()
{
    if (ImGui::TreeNode(getComponentName().c_str()))
    {
        ImGui::Text(shaderName.c_str());
        ImGui::ColorEdit3("Default Color", (float*)&defaultColor);

        ImGui::TreePop();
    }
}


void baseShader::blind_shader_value()
{
    Shader::use();
    //texture
    blindTexturesChannel();

    /////////////////////////
    //must pass these
    setMat4("projection", globalParametersManager::getInstance().getProjection());
    setMat4("view", globalParametersManager::getInstance().mainCamera->GetViewMatrix());
    setMat4("model", object->GetComponent<Transform>()->getModelMat4());
    /////////////////////////

    glm::vec3 temp = glm::vec3(defaultColor.x, defaultColor.y, defaultColor.z);
    setVec3("defaultColor", temp);
}


void baseShader::setShaderName(std::string& _ShaderName)
{
    shaderName = _ShaderName;
}

void baseShader::setShaderName(std::string&& _ShaderName)
{
    shaderName = _ShaderName;
}


const std::string baseShader::getComponentName() const
{
    return componentName;
}

nlohmann::json baseShader::toJson()
{
    nlohmann::json ret;
    nlohmann::json data;

    data["defaultColor"] = vec4ToJson(imToGlmVec4(defaultColor));

    ret["componentName"] = getComponentName();
    ret["componentData"] = data;

    return ret;
}

void baseShader::loadJson(const nlohmann::json& js)
{
    defaultColor=jsonToImVec4(js);
}

std::unique_ptr<Component> baseShader::clone() const
{
    return std::make_unique<baseShader>(*this);
}



