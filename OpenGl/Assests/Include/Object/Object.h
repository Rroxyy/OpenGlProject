#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Transform.h"

class Component;
class Object {
public:
    std::string objectName;
    Transform* transform;

    Object();
    ~Object();

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args);

    template <typename T>
    T* Object::AddComponent(std::unique_ptr<T> comp);
	
    template<typename T>
    T* GetComponent();

private:

    std::unordered_map<size_t, std::unique_ptr<Component>> components;
};
