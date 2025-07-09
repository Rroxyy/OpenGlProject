#pragma once
#include <functional>
#include <memory>
#include <string>

#include "Component.h"
#include "Transform.h"
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
    }
};

static ComponentFactoryRegisterHelper helper;