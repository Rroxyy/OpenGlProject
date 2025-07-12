#include "baseShader.h"

#include "camera.h"
#include "globalParametersManager.h"
#include "Object.h"
#include "ResourcePathManager.h"
#include "UI_Manager.h"
#include "plugins.h"

baseShader::baseShader(const char* vertexPath, const char* fragmentPath)
	: Shader(vertexPath, fragmentPath)
{
    componentName = "BaseShader";
	
}

baseShader::baseShader()
	: baseShader(ResourcePathManager::getInstance().getBaseShaderVert().c_str(),
		ResourcePathManager::getInstance().getBaseShaderFrag().c_str())
{
	
}

baseShader::baseShader(std::string&& _shaderName):
	baseShader(ResourcePathManager::getInstance().getBaseShaderVert().c_str(),
		ResourcePathManager::getInstance().getBaseShaderFrag().c_str())
{
	shaderName = _shaderName;
}

void baseShader::update() 
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



void baseShader::showUI()
{
    if (ImGui::TreeNode(getComponentName().c_str()))
    {
        ImGui::Text(shaderName.c_str());
        ImGui::ColorEdit3("Default Color", (float*)&defaultColor);

        ImGui::TreePop();
    }
}


void baseShader::update_shader_value()
{
    
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



