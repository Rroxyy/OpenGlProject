#pragma once

#include "Component.h"
#include <glm/glm.hpp>  

class Transform : public Component {
public:
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
    glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };  // 欧拉角，维护san值

    Transform()
    {
        componentName = "Transform";
    }

    Transform(const glm::vec3& pos, const glm::vec3& scl, const glm::vec3& rot)
        : position(pos), scale(scl), rotation(rot) {
    }

    const std::string& getComponentName() const override
    {
        return componentName;
    }

    nlohmann::json toJson() override
    {
        nlohmann::json ret;
        nlohmann::json data;
        data["position"] = vec3ToJson(position);
        data["scale"] = vec3ToJson(scale);
        data["rotation"] = vec3ToJson(rotation);
        ret[getComponentName()] = data;
        return ret;
    }

    nlohmann::json vec3ToJson(const glm::vec3& v) const {
        return { {"x", v.x}, {"y", v.y}, {"z", v.z} };
    }


};

