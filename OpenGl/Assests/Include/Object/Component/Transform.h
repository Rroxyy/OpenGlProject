#pragma once

#include <imgui.h>
#include <iostream>
#include <json.hpp>
#include <memory>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"


#include "GodClass.h"
#include "imgui/ImGuizmo.h"
#include"camera.h"
#include "InputSystem.h"
#include "Object.h"

class ModelComponent;

class Transform : public Component {
public:
    bool dirty;
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
    glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };  // 欧拉角，维护san值

    glm::vec3 forward{ 0.0f,0.0f,-1.0f };

    glm::mat4 modelMat = glm::mat4(1.0f);

    Transform()
    {
        componentName = "Transform";
        dirty = false;
    }

    

    Transform(const glm::vec3& pos, const glm::vec3& scl, const glm::vec3& rot)
        : position(pos), scale(scl), rotation(rot) {
        componentName = "Transform";
        dirty = true;
    }

    ~Transform()
    {

    }

    glm::mat4 getModelMat4()
    {
        dirtyCheck();
        return modelMat;
    }

    
    glm::vec3 getForward()
    {
        dirtyCheck();
        return forward;
    }

    glm::vec3 getBackDir()
    {
        return -getForward();
    }

    const std::string getComponentName() const override
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
        dirty = true;
    }

   

    void showUI() override
    {
        dirty = true;

        if (ImGui::TreeNode(getComponentName().c_str()))
        {
            ImGui::DragFloat3("Position", &position.x, 0.1f);
            ImGui::DragFloat3("Rotation", &rotation.x, 0.1f);
            ImGui::DragFloat3("Scale", &scale.x, 0.1f);
            ImGui::TreePop();
        }


    }


    void drawManipulate()
    {
        // 编辑器 Gizmo 绘制逻辑
        ImGuiIO& io = ImGui::GetIO();
        ImGuizmo::BeginFrame();

        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        ImGuizmo::SetOrthographic(false);

        ImGuizmo::Manipulate(
            glm::value_ptr(GodClass::getInstance().getMainCamera()->GetViewMatrix()),
            glm::value_ptr(GodClass::getInstance().getProjection()),
            InputSystem::getInstance().operation,  
            ImGuizmo::WORLD,
            glm::value_ptr(modelMat)
        );

        if (ImGuizmo::IsUsing())
        {
            updateFromModelMatrix();  // ←←← 如果有拖动操作，就反解更新 transform
        }
    }



     nlohmann::json vec3ToJson(const glm::vec3& v) const {
        return { {"x", v.x}, {"y", v.y}, {"z", v.z} };
    }
private:
    void dirtyCheck()
    {
        if (dirty)
        {
            dirty = false;

            // 初始化为单位矩阵
            modelMat = glm::mat4(1.0f);

            // 平移
            modelMat = glm::translate(modelMat, position);


            // 旋转（假设 rotation 是欧拉角，单位为弧度）
            modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));


            // 缩放
            modelMat = glm::scale(modelMat, scale);


            forward = -glm::normalize(glm::vec3(modelMat[2]));

            updateAABB();
        }
    }
    void updateFromModelMatrix()
    {
        float matrix[16];
        memcpy(matrix, glm::value_ptr(modelMat), sizeof(float) * 16);

        float translation[3], rotationDeg[3], scale[3];

        ImGuizmo::DecomposeMatrixToComponents(matrix, translation, rotationDeg, scale);

        position = glm::vec3(translation[0], translation[1], translation[2]);
        rotation = glm::radians(glm::vec3(rotationDeg[0], rotationDeg[1], rotationDeg[2])); // 转为弧度
        this->scale = glm::vec3(scale[0], scale[1], scale[2]);

        /*dirty = true;          
        dirtyCheck();       */   

    }

    void updateAABB();
   
};

