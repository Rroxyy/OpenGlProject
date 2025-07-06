#pragma once

#include "Component.h"
#include <glm/glm.hpp>  

class Transform : public Component {
public:
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
    glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };  // 欧拉角，维护san值

    Transform() = default;

    Transform(const glm::vec3& pos, const glm::vec3& scl, const glm::vec3& rot)
        : position(pos), scale(scl), rotation(rot) {
    }

    
};

