#include "Object.h"

#include <string>

#include "json.hpp"
#include "ObjectManager.h"
#include "ComponentFactory.h"
#include "UI_Manager.h"

Object::Object()
{
    objectName = "Empty Object";
    AddComponent<Transform>();
    UIManager::getInstance().Register([this]()
        {
            this->showUI();
        });
}

Object::~Object()
{}

void Object::use()
{
    for (const auto& it : components)it.second.get()->use();
}


void Object::showUI()
{
	if (ImGui::TreeNode((getObjectName()+ std::to_string(id)).c_str()))
	{
        for (auto& it:components)
        {
            it.second->showUI();
        }

        ImGui::TreePop();
	}
}

nlohmann::json Object::toJson() 
{
    nlohmann::json ret;
    nlohmann::json data = nlohmann::json::array();  
    for (auto& it:components)
    {
        auto component = it.second.get();
        //std::cout << component->getComponentName() << std::endl;
        data.push_back(component->toJson());
    }
    ret["componentsData"] = data;
    ret["id"] = id;
    ret["objectName"] = objectName;

    return ret;
}

void Object::loadJson(const nlohmann::json& componentsJson)
{
	for (auto& js:componentsJson)
	{
        std::string name = js["componentName"];
        auto comPtr=ComponentFactory::getInstance().create(name);
        auto ptr= AddComponent(std::move(comPtr));
        ptr->loadJson(js["componentData"]);
        
	}
}

void  Object::setObjectName(std::string&& name)
{
    objectName = name;
}

const std::string& Object::getObjectName()const
{
    return objectName;
}


