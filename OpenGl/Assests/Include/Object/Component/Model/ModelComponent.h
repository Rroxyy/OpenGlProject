#pragma once
#include "Component.h"
#include "ModelResource.h"
#include "ResourceManager.h"
#include "Transform.h"

class ModelComponent : public Component {
public:
    AABB aabb;

    ModelComponent() {
        componentName = "ModelComponent";
    }

    ModelComponent(std::shared_ptr<ModelResource> resource) : model(std::move(resource)) {
        componentName = "ModelComponent";
    }

    void resetModel(std::shared_ptr<ModelResource> resource)
    {
        model = std::move(resource);
        resetAABB();
    }

    // 渲染
    void Draw() {
        if (model) model->Draw();
    }

    std::string getModelPath() const
    {
        return model ? model->filePath : "null";
    }


    const std::string getComponentName() const override {
        return componentName;
    }

    void setObject(Object* objectptr) override {
        object = objectptr;
        resetAABB();
    }

    void resetAABB() {
        if (!model || !object) return;
        auto* transform = object->GetComponentExact<Transform>();
        if (!transform) return;

        aabb = model->localAABB.translateToWorld_AABB(transform->getModelMat4());
    }

    void showUI() override {
        if (ImGui::TreeNode(getComponentName().c_str())) {
            aabb.showUI();
            if (model) model->showUI();
            ImGui::TreePop();
        }
    }

    std::unique_ptr<Component> clone() const override {
        return std::make_unique<ModelComponent>(model);
    }

    void start() override
    {
        resetAABB();
    }

    nlohmann::json toJson() override {
        nlohmann::json ret;
        ret["componentName"] = getComponentName();
        nlohmann::json data;
        data["filePath"] = model ? model->filePath : "";
        ret["componentData"] = data;
        return ret;
    }

    void loadJson(const nlohmann::json& js) override {
        std::string path = js["filePath"];
        model = ResourceManager::getInstance().loadModel(path);
        resetAABB();
    }

private:
    std::shared_ptr<ModelResource> model; // 共享模型资源

};
