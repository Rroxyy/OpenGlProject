#pragma once
#include <json.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include "Transform.h"

class Component;
class Object {
public:
    size_t id;
    std::string objectName;

    Object();
    ~Object();

    template <typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        // 编译期断言：确保模板类型 T 是 Component 的派生类（或 Component 本身）
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

        // 使用完美转发构造一个 T 类型的对象，并由 unique_ptr 接管内存管理
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        //auto comp = std::make_unique<T>(static_cast<Args&&>(args)...);

        // 获取裸指针，以便返回给外部使用
        T* ptr = comp.get();

        // 将组件存入对象的组件容器，key 使用类型的哈希值（typeid 获取类型信息）
        components[typeid(T).hash_code()] = std::move(comp);

        // 返回组件的裸指针，方便调用者继续操作这个组件
        return ptr;
    }

    template <typename T>
    T* AddComponent(std::unique_ptr<T> comp)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        T* ptr = comp.get();
        components[typeid(T)] = std::move(comp);
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

    virtual nlohmann::json toJson() ;

    virtual const std::string& getObjectName()const;
private:
    std::unordered_map<size_t, std::unique_ptr<Component>> components;
};
