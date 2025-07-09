#pragma once

#include <imgui.h>

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

    std::unique_ptr<Component> clone() const override {
        return std::make_unique<Transform>(*this);  // 调用默认拷贝构造
    }

    nlohmann::json toJson() override
    {
        nlohmann::json ret;
        nlohmann::json data;

        data["position"] = vec3ToJson(position);
        data["scale"] = vec3ToJson(scale);
        data["rotation"] = vec3ToJson(rotation);
        ret["componentName"] = getComponentName();
        ret["componentData"] = data;
        return ret;
    }

    void loadJson(const nlohmann::json& js) override
    {
        position.x = js["position"]["x"].get<float>();
        position.y = js["position"]["y"].get<float>();
        position.z = js["position"]["z"].get<float>();
        rotation.x = js["rotation"]["x"].get<float>();
        rotation.y = js["rotation"]["y"].get<float>();
        rotation.z = js["rotation"]["z"].get<float>();
        scale.x = js["scale"]["x"].get<float>();
        scale.y = js["scale"]["y"].get<float>();
        scale.z = js["scale"]["z"].get<float>();
    }

    void showUI() override
    {
        if (ImGui::TreeNode(getComponentName().c_str()))
        {
            ImGui::DragFloat3("Position", &position.x, 0.1f);
            ImGui::DragFloat3("Rotation", &rotation.x, 0.1f);
            ImGui::DragFloat3("Scale", &scale.x, 0.1f);

            ImGui::TreePop();
        }
    }

     nlohmann::json vec3ToJson(const glm::vec3& v) const {
        return { {"x", v.x}, {"y", v.y}, {"z", v.z} };
    }
};

