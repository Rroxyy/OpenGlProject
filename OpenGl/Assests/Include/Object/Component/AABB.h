#pragma once
#include <glm/glm.hpp>
#include <limits>
#include <algorithm>
#include <iostream>
#include "Ray.h"
class AABB
{
public:
    AABB()
    {
        reset();
    }

    void showUI()
    {
        if (ImGui::TreeNode("AABB"))
        {
            ImGui::Text("Axis-Aligned Bounding Box");

            ImGui::Text("Min: (%.2f, %.2f, %.2f)", min_AABB.x, min_AABB.y, min_AABB.z);
            ImGui::Text("Max: (%.2f, %.2f, %.2f)", max_AABB.x, max_AABB.y, max_AABB.z);

            glm::vec3 center = (min_AABB + max_AABB) * 0.5f;
            glm::vec3 size = max_AABB - min_AABB;

            ImGui::Separator();
            ImGui::Text("Center: (%.2f, %.2f, %.2f)", center.x, center.y, center.z);
            ImGui::Text("Size:   (%.2f, %.2f, %.2f)", size.x, size.y, size.z);

            ImGui::TreePop();
        }
    }

    // 重置为无效的盒子（等于清空）
    void reset()
    {
        min_AABB = glm::vec3(std::numeric_limits<float>::max());
        max_AABB = glm::vec3(std::numeric_limits<float>::lowest());
    }

    // 扩展当前 AABB，使其包含该点
    void updateAABB(const glm::vec3& point)
    {
        min_AABB = glm::min(min_AABB, point);
        max_AABB = glm::max(max_AABB, point);
    }

    // 合并另一个 AABB（取包围两者的最小盒）
    void merge(const AABB& other)
    {
        min_AABB = glm::min(min_AABB, other.min_AABB);
        max_AABB = glm::max(max_AABB, other.max_AABB);
    }

    // 获取盒子的中心点
    glm::vec3 getCenter() const
    {
        return (min_AABB + max_AABB) * 0.5f;
    }

    // 获取盒子的尺寸（宽高深）
    glm::vec3 getSize() const
    {
        return max_AABB - min_AABB;
    }

    // 判断一个点是否在 AABB 内部
    bool contains(const glm::vec3& point) const
    {
        return (point.x >= min_AABB.x && point.x <= max_AABB.x) &&
            (point.y >= min_AABB.y && point.y <= max_AABB.y) &&
            (point.z >= min_AABB.z && point.z <= max_AABB.z);
    }

    // 判断两个 AABB 是否相交
    bool intersects(const AABB& other) const
    {
        return (min_AABB.x <= other.max_AABB.x && max_AABB.x >= other.min_AABB.x) &&
            (min_AABB.y <= other.max_AABB.y && max_AABB.y >= other.min_AABB.y) &&
            (min_AABB.z <= other.max_AABB.z && max_AABB.z >= other.min_AABB.z);
    }

    // 射线与 AABB 相交测试（返回 true 表示有交点，输出 tNear/tFar）
    bool intersectsRay(const Ray& ray, float& tNear, float& tFar) const
    {
        tNear = -std::numeric_limits<float>::infinity();
        tFar = std::numeric_limits<float>::infinity();

        for (int i = 0; i < 3; ++i) // x, y, z
        {
            float origin = ray.origin[i];
            float direction = ray.rayDir[i];
            float invD = 1.0f / direction;

            float t1 = (min_AABB[i] - origin) * invD;
            float t2 = (max_AABB[i] - origin) * invD;

            if (t1 > t2) std::swap(t1, t2);

            tNear = std::max(tNear, t1);
            tFar = std::min(tFar, t2);

            if (tNear > tFar) return false; // 无交点
        }

        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const AABB& box)
    {
        os << "AABB("
            << "Min: (" << box.min_AABB.x << ", " << box.min_AABB.y << ", " << box.min_AABB.z << "), "
            << "Max: (" << box.max_AABB.x << ", " << box.max_AABB.y << ", " << box.max_AABB.z << ")"
            << ")";
        return os;
    }
private:
    glm::vec3 min_AABB;
    glm::vec3 max_AABB;
};
