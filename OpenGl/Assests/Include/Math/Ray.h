#pragma once
#include <glm/glm.hpp>
#include <limits>

class Ray
{
public:
    glm::vec3 origin;   // 起点
    glm::vec3 rayDir;   // 射线方向（需归一化）

    // 默认构造函数
    Ray()
        : origin(0.0f), rayDir(0.0f, 0.0f, 1.0f) // 默认向Z轴方向
    {
    }

    // 参数构造函数
    Ray(const glm::vec3& o, const glm::vec3& dir)
        : origin(o), rayDir(glm::normalize(dir))
    {
    }

    // 获取射线在 t 处的点
    glm::vec3 getPoint(float t) const
    {
        return origin + rayDir * t;
    }

   

   
};
