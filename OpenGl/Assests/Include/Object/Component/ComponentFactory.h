#pragma once
#include <functional>
#include <memory>
#include <string>

#include "baseShader.h"
#include "Component.h"
#include "mesh.h"
#include "ModelComponent.h"
#include "Render.h"
#include "Transform.h"
#include "Shaders/BaseLightShader.h"

using ComponentCreator = std::function<std::unique_ptr<Component>()>;

class ComponentFactory {
private:
    std::unordered_map<std::string, ComponentCreator> creators;

public:
    static ComponentFactory& getInstance() {
        static ComponentFactory factory;
        return factory;
    }

    void registerComponent(const std::string& type, ComponentCreator creator) {
        creators[type] = std::move(creator);
    }

    std::unique_ptr<Component> create(const std::string& type) {
        auto it = creators.find(type);
        if (it != creators.end()) {
            return it->second()->clone();
        }
        return nullptr;
    }
};


class ComponentFactoryRegisterHelper {
public:
    ComponentFactoryRegisterHelper() {
        ComponentFactory::getInstance().registerComponent("Transform", []() {
            return std::make_unique<Transform>();
            });

        ComponentFactory::getInstance().registerComponent("ModelComponent", []() {
            return std::make_unique<ModelComponent>();
            });

        ComponentFactory::getInstance().registerComponent("Render", []() {
            return std::make_unique<Render>();
            });







        //shader

        ComponentFactory::getInstance().registerComponent("baseShader", []() {
            return std::make_unique<baseShader>();
            });

        ComponentFactory::getInstance().registerComponent("BaseLightShader", []() {
            return std::make_unique<BaseLightShader>();
            });
    }
};

static ComponentFactoryRegisterHelper helper;