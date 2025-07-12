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
	// 组件类型及推荐执行顺序（executionOrder 越小越早执行）
/*
	ComponentType     executionOrder
	Shader            0
	Camera            1
	Light             2
	Mesh              10
	Particle          20
*/
	unsigned int executionOrder = 10;

	Object* object=nullptr;
	virtual const std::string getComponentName()const=0;
	virtual nlohmann::json toJson() = 0;
	virtual void loadJson(const nlohmann::json& js) = 0;
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

private:
};
