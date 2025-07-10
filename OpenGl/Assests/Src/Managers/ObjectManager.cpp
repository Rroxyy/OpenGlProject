#include "ObjectManager.h"

#include <iostream>
#include <fstream>
#include "Object.h"

ObjectManager::~ObjectManager()
{
	saveJson();
}


size_t ObjectManager::createObject()
{
	objectCnt++;
	size_t id = objectCnt;

	//wrong
	//auto obj = std::make_unique<Object>();

	//ok
	std::unique_ptr<Object> obj(new Object());
	obj->id = id;

	objects[id] = std::move(obj);
	return id;
}

Object* ObjectManager::createObjectPtr()
{
	objectCnt++;
	size_t id = objectCnt;

	std::unique_ptr<Object> obj(new Object());
	
	obj->id = id;

	objects[id] = std::move(obj);
	return objects[id].get();
}

Object* ObjectManager::createObjectPtr(std::string&& name)
{
	Object* object = createObjectPtr();
	object->objectName = name;
	return object;
}


void ObjectManager::createObjectByJson(const nlohmann::json& js)
{
	objectCnt++;
	size_t id = objectCnt;
	std::unique_ptr<Object> obj(new Object());
	objects[id] = std::move(obj);
	Object* newObject = objects[id].get();

	newObject->id = id;
	newObject->objectName = js["objectName"];
	newObject->loadJson(js["componentsData"]);

}



Object* ObjectManager::getObject(size_t id)
{
	auto it = objects.find(id);
	return (it != objects.end()) ? it->second.get() : nullptr;

}

bool ObjectManager::removeObject(size_t id)
{
	auto it = objects.find(id);
	if (it != objects.end()) {
		objects.erase(it);  
		return true;
	}
	return false;  
}

bool ObjectManager::removeObject(Object* object)
{
	return removeObject(object->id);
}

void ObjectManager::saveJson()
{
	nlohmann::json js=nlohmann::json::array();
	for (auto& it:objects)
	{
		auto ptr = it.second.get();
		js.push_back(ptr->toJson());
	}
	std::string path = "C:\\Users\\Drwin\\Desktop\\render\\render3\\vs\\OpenGl\\Assests\\Resource\\ObjectData\\ObjectManager.json";

	// 打开文件，std::ios::trunc 表示清空旧内容
	std::ofstream file(path, std::ios::out | std::ios::trunc);
	if (!file.is_open()) {
		std::cerr << "无法打开文件用于写入: " << path << std::endl;
		std::abort();
	}

	// 写入 json 内容（默认带缩进）
	file << js.dump(4);  // 4 表示缩进 4 个空格，便于阅读
	file.close();
}

void ObjectManager::loadJson()
{
	std::string path = "C:\\Users\\Drwin\\Desktop\\render\\render3\\vs\\OpenGl\\Assests\\Resource\\ObjectData\\ObjectManager.json";

	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "无法打开文件用于读取: " << path << std::endl;
		std::abort();
	}

	nlohmann::json js;
	file >> js;  // 解析 JSON 文件
	file.close();

	if (!js.is_array()) {
		std::cerr << "JSON 文件格式错误，预期是数组" << std::endl;
		std::abort();
	}

	// 清空当前对象容器（如果需要）
	objects.clear();
	objectCnt = 0;

	for (const auto& objJson : js) {
		// 这里假设Object有静态函数fromJson返回Object指针或unique_ptr
		createObjectByJson(objJson);
		
	}
}




