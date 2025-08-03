#pragma once
#include <json.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

#include "Transform.h"

class Component;
class Object {
    friend class Scene;
public:
    size_t id;
    std::string objectName;
    bool checkAABB = true;

   
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
                << "  [警告] Component 类型 " << typeid(*comp).name()
                << " 已存在，取消新建，返回已有实例。\n";

            return static_cast<T*>(it->second.get());
        }

        T* ptr = comp.get();
        ptr->setObject(this);
        components[key] = std::move(comp);
        

        return ptr;
    }

    template<typename T>
    T* GetComponentExact()
    {
        const auto& it = components.find(typeid(T).hash_code());
        if (it == components.end())return nullptr;
        return static_cast<T*>(it->second.get());
    }
    template <typename T>
    T* GetComponentAs()
    {
        for (auto& [typeHash, component] : components)
        {
            T* casted = dynamic_cast<T*>(component.get());
            if (casted)
                return casted; // 找到类型为 T 或其派生类的组件
        }
        return nullptr; // 没找到
    }

    void start();
    void beforeUpdate();
    void update();
    void afterUpdate();


    nlohmann::json toJson();
    void loadJson(const nlohmann::json& componentsJson);
    const std::string& getObjectName()const;
    void setObjectName(std::string&& name);
    void showUI();


    void Unfocus();
	void Focus();
   
    void SetSave(bool _save) { NEEDSAVE = _save; }
    bool NeedSave()const { return NEEDSAVE; }
private:
    bool NEEDSAVE = true;
    bool isFocus = false;
    Object();
    Object(bool isCreateByJson);

    std::unordered_map<size_t, std::unique_ptr<Component>> components;
};
