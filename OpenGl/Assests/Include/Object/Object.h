#pragma once
#include <json.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include "Transform.h"

class Component;
class Object {
    friend class ObjectManager;
public:
    size_t id;
    std::string objectName;

   
    ~Object();

    template <typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        return AddComponent(std::move(comp));
    }


    template <typename T>
    T* AddComponent(std::unique_ptr<T> comp)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

        size_t key = typeid(*comp).hash_code();


        auto it = components.find(key);

        if (it != components.end()) {
            std::cerr << "Object: " << objectName<<id
                << "  [警告] Component 类型 " << typeid(T).name()
                << " 已存在，取消新建，返回已有实例。\n";

            return static_cast<T*>(it->second.get());
        }

        T* ptr = comp.get();
        ptr->setObject(this);
        components[key] = std::move(comp);
        

        return ptr;
    }

    template <typename T>
    T* GetComponent()
    {
        auto it = components.find(typeid(T).hash_code());
        if (it != components.end()) {
            return dynamic_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    nlohmann::json toJson();

    void loadJson(const nlohmann::json& componentsJson);

    const std::string& getObjectName()const;

    void setObjectName(std::string&& name);

    void showUI();

private:

    Object();
    std::unordered_map<size_t, std::unique_ptr<Component>> components;
};
