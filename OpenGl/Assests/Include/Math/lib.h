#pragma once
#include <algorithm>
#include <glm/detail/type_vec.hpp>
#include <glm/gtc/quaternion.hpp>

//弧度，不需要传入单位化向量
float GetAngle(const glm::vec3& dir1, const glm::vec3& dir2)
{
    float len1 = glm::length(dir1);
    float len2 = glm::length(dir2);
    if (len1 == 0.0f || len2 == 0.0f) {
        // 角度对零向量未定义，你可以选择返回 0 或 NaN
        return 0.0f;
        // 或者： return std::numeric_limits<float>::quiet_NaN();
    }

    float cosTheta = glm::dot(dir1, dir2) / (len1 * len2);
    // 防止因浮点误差导致cosTheta略微超出[-1,1]
    cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

    return std::acos(cosTheta); // 单位：弧度 [0, π]
}