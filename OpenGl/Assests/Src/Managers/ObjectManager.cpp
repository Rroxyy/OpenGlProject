#include "ObjectManager.h"

#include <iostream>
#include <fstream>
#include "Object.h"

size_t ObjectManager::addObject(Object* object)
{
	if (!object) return 0; // 或抛异常，防止空指针
	objectCnt++;
	size_t id = objectCnt;
	objects[id] = std::unique_ptr<Object>(object);
	return static_cast<unsigned int>(id);

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


