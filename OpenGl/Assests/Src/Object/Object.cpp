#include "Object.h"

#include <string>

#include "json.hpp"
#include "Scene.h"
#include "ComponentFactory.h"
#include "UI_Manager.h"

Object::Object()
{
    objectName = "Empty Object";
    UIManager::getInstance().RegisterForMainWindow([this]()
        {
            this->showUI();
        });
}

Object::Object(bool isCreateByJson):Object()
{
	if (isCreateByJson==false)
    {
        AddComponent<Transform>();
    }
}


Object::~Object()
{
    if (isFocus)Unfocus();
    std::cout << "Release Object: " << objectName << std::endl;
}

void Object::start()
{
    for (const auto& it : components)it.second.get()->start();
}

void Object::beforeUpdate()
{
    for (const auto& it : components)it.second.get()->beforeUpdate();
}

void Object::update()
{
    for (const auto& it : components)it.second.get()->update();

}

void Object::afterUpdate()
{
    for (const auto& it : components)it.second.get()->afterUpdate();

  
}





void Object::showUI()
{
   
    if (ImGui::Button((getObjectName() + std::to_string(id)).c_str()))
    {
        if (isFocus)
        {
            Unfocus();
        }
        else
        {
            Focus();
        }
    }

    if (isFocus)
    {
        std::string windowName = getObjectName() + std::to_string(id);

        // 注意：传 &isFocus，用户点 X 时 ImGui 会把它改为 false
        if (ImGui::Begin(windowName.c_str(), &isFocus))
        {
            for (auto& it : components)
            {
                it.second->showUI();
            }
        }

        GetComponentExact<Transform>()->drawManipulate();
        ImGui::End();
    }
    //std::cout << objectName << " :" << isFocus << std::endl;
   
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
        if (js.contains("componentName") && js["componentName"].is_string()) {
            std::string name = js["componentName"].get<std::string>();
            //std::cout << getObjectName() << " " << name << std::endl;

            std::unique_ptr<Component> comPtr = ComponentFactory::getInstance().create(name);
            auto ptr = AddComponent(std::move(comPtr));
            ptr->loadJson(js["componentData"]);

            //std::cout << getObjectName() << " " << name << std::endl;
        }
        else {
            std::cerr << "Object::loadJson: 缺少或错误的字段 componentName: " << js.dump(4) << std::endl;
            std::abort();
        }
        
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


void Object::Unfocus()
{
    isFocus = false;
    Scene::getInstance().removeFocusedObj(this);
}


void Object::Focus()
{
        isFocus = true;
        Scene::getInstance().addFocusedObj(this);
}


