#pragma once

#include "json.hpp"

class Object;
class Component
{
public:
	virtual ~Component()
	{
		std::cout << "Class Component:               " << "Release component: " << componentName << std::endl;
	}
	std::string componentName;

	Object* object=nullptr;
	virtual const std::string getComponentName()const=0;
	
	virtual std::unique_ptr<Component> clone() const = 0;
	virtual void showUI() = 0;
	virtual void setObject(Object* objectptr)
	{
		object = objectptr;
	}

	virtual  void start() {};
	virtual void beforeUpdate() {};
	virtual void update() {};
	virtual void afterUpdate() {};

	virtual nlohmann::json toJson() = 0;
	virtual void loadJson(const nlohmann::json& js) = 0;

private:
};
