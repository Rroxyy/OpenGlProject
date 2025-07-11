#pragma once
#include <json.hpp>
#include <glm/detail/type_vec.hpp>


static nlohmann::json vec3ToJson(const glm::vec3& v)
{
    return { {"x", v.x}, {"y", v.y}, {"z", v.z} };
}

static nlohmann::json vec2ToJson(const glm::vec2& v)
{
    return { {"x", v.x}, {"y", v.y} };
}

static glm::vec3 jsonToVec3(const nlohmann::json& j)
{
    return glm::vec3(j.at("x").get<float>(), j.at("y").get<float>(), j.at("z").get<float>());
}

static glm::vec2 jsonToVec2(const nlohmann::json& j)
{
    return glm::vec2(j.at("x").get<float>(), j.at("y").get<float>());
}