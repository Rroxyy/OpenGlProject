#pragma once
#include <json.hpp>
#include <glm/vec4.hpp>   // glm::vec4
#include <imgui.h>        // ImVec4

// === glm::vec4 <-> json ===
static nlohmann::json vec4ToJson(const glm::vec4& v)
{
    return { {"x", v.x}, {"y", v.y}, {"z", v.z}, {"w", v.w} };
}

static glm::vec4 jsonToVec4(const nlohmann::json& j)
{
    return glm::vec4(
        j.at("x").get<float>(),
        j.at("y").get<float>(),
        j.at("z").get<float>(),
        j.at("w").get<float>()
    );
}

// === glm::vec4 <-> ImVec4 ===
static ImVec4 glmToImVec4(const glm::vec4& v)
{
    return ImVec4(v.x, v.y, v.z, v.w);
}

static glm::vec4 imToGlmVec4(const ImVec4& v)
{
    return glm::vec4(v.x, v.y, v.z, v.w);
}

// === ImVec4 <-> json ===
static nlohmann::json imVec4ToJson(const ImVec4& v)
{
    return { {"x", v.x}, {"y", v.y}, {"z", v.z}, {"w", v.w} };
}

static ImVec4 jsonToImVec4(const nlohmann::json& j)
{
    return ImVec4(
        j.at("x").get<float>(),
        j.at("y").get<float>(),
        j.at("z").get<float>(),
        j.at("w").get<float>()
    );
}

static nlohmann::json vec3ToJson(const glm::vec3& v)
{
    return { {"x", v.x}, {"y", v.y}, {"z", v.z} };
}

static glm::vec3 jsonToVec3(const nlohmann::json& j)
{
    return glm::vec3(
        j.at("x").get<float>(),
        j.at("y").get<float>(),
        j.at("z").get<float>()
    );
}

static nlohmann::json vec2ToJson(const glm::vec2& v)
{
    return { {"x", v.x}, {"y", v.y} };
}

static glm::vec2 jsonToVec2(const nlohmann::json& j)
{
    return glm::vec2(
        j.at("x").get<float>(),
        j.at("y").get<float>()
    );
}








/////////////////////////////////file///////////////////////////////////////
inline void loadFileToString(const char* fragmentPath, std::string& outContent)
{
    std::ifstream fShaderFile;
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    outContent.clear();
    try {
        fShaderFile.open(fragmentPath);
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        outContent = fShaderStream.str().c_str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FRAGMENT_FILE_NOT_READ: " << e.what() << std::endl;
    }
}
