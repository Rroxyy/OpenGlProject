#include "Object.h"

#include <string>

#include "json.hpp"
#include "ObjectManager.h"


Object::Object()
{
    id=ObjectManager::getInstance().addObject(this);
    AddComponent()
}

Object::~Object()
{}

nlohmann::json Object::toJson() 
{
    nlohmann::json ret;
    nlohmann::json data = nlohmann::json::array();  
    for (auto& it:components)
    {
        auto component = it.second.get();
        data.push_back(component->toJson());
    }
    ret["componentsData"] = data;
    ret["id"] = id;
    ret["name"] = objectName;

    return ret;
}

const std::string& Object::getObjectName()const
{
    return objectName;
}


