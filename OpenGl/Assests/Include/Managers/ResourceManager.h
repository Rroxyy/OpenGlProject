#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>

#include "ModelResource.h"
#include "QuadMesh.h"

class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // 通过路径加载模型，如果已存在则复用
    std::shared_ptr<ModelResource> loadModel(const std::string& path) {
        auto it = modelByPath.find(path);
        if (it != modelByPath.end()) {
            return it->second;
        }

        auto model = std::make_shared<ModelResource>(path);
        if (model->meshes.empty()) {
            std::cerr << "Failed to load model from: " << path << std::endl;
            return nullptr;
        }

        modelByPath[path] = model;
        return model;
    }

    // 注册手动创建的模型资源
    std::shared_ptr<ModelResource> registerModel(const std::string& name, std::shared_ptr<ModelResource> model) {
        modelByName[name] = model;
        return model;
    }

    // 获取手动注册或路径加载的模型
    std::shared_ptr<ModelResource> getModel(const std::string& nameOrPath) const {
        auto it = modelByName.find(nameOrPath);
        if (it != modelByName.end())
            return it->second;

        auto it2 = modelByPath.find(nameOrPath);
        if (it2 != modelByPath.end())
            return it2->second;

        return nullptr;
    }

    // 判断是否存在某个模型
    bool hasModel(const std::string& nameOrPath) const {
        return modelByName.count(nameOrPath) || modelByPath.count(nameOrPath);
    }

    // 清空所有缓存
    void clearAll() {
        modelByPath.clear();
        modelByName.clear();
    }

     QuadMesh* getQuadMesh() { return quadMesh.get(); }

private:
    ResourceManager()
    {
        quadMesh = std::make_unique<QuadMesh>();
    }
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    std::unordered_map<std::string, std::shared_ptr<ModelResource>> modelByPath;
    std::unordered_map<std::string, std::shared_ptr<ModelResource>> modelByName;

    std::unique_ptr<QuadMesh> quadMesh;
};
