#include "Scene.h"

#include <iostream>
#include <fstream>
#include <unordered_set>

#include "lib.h"
#include "ModelComponent.h"
#include "ModelResource.h"
#include "Object.h"
#include "Ray.h"

Scene::~Scene()
{
	clearFocusObj();
	saveJson();
}


void Scene::start()
{
	for (auto& it : objects)
	{
		it.second.get()->start();
	}
}

void Scene::beforeUpdate()
{
	for (auto& it : objects)
	{
		it.second.get()->beforeUpdate();
	}
}


void Scene::update()
{
	
	for (auto&it:objects)
	{
		it.second.get()->update();
	}

}


void Scene::afterUpdate()
{
	for (auto& it : objects)
	{
		it.second.get()->afterUpdate();
	}
}




size_t Scene::createObject()
{
	objectIndex++;
	

	while (objects.find(objectIndex) != objects.end())
	{
		objectIndex++;
	}

	std::unique_ptr<Object> obj(new Object(false));
	objectList.push_back(obj.get());
	obj->id = objectIndex;

	objects[objectIndex] = std::move(obj);
	return objectIndex;
}

Object* Scene::createObjectPtr()
{
	size_t id = createObject();
	return objects[id].get();
}

Object* Scene::createObjectPtr(std::string&& name)
{
	Object* object = createObjectPtr();
	object->objectName = name;
	return object;
}


void Scene::createObjectByJson(const nlohmann::json& js)
{
	std::unique_ptr<Object> obj(new Object(true));
	Object* newObject = obj.get();
	newObject->id = js["id"];
	newObject->objectName = js["objectName"];

	objectList.push_back(obj.get());
	objects[obj->id] = std::move(obj);

	newObject->loadJson(js["componentsData"]);

}



Object* Scene::getObject(size_t id)
{
	auto it = objects.find(id);
	return (it != objects.end()) ? it->second.get() : nullptr;

}

bool Scene::removeObject(size_t id)
{
	auto it = objects.find(id);
	if (it != objects.end()) {
		objects.erase(it);  
		return true;
	}
	return false;  
}

bool Scene::removeObject(Object* object)
{
	if (object == nullptr)return false;
	return removeObject(object->id);
}




void Scene::addFocusedObj(Object* obj)
{
	if (obj == nullptr)return;
	focusObjects.insert(obj);
}
void Scene::removeFocusedObj(Object* obj)
{
	if (!focusObjects.count(obj))return;
	focusObjects.erase(obj);
}
void Scene::clearFocusObj()
{
	if (focusObjects.empty()) return;

	// ✅ 拷贝一份，避免遍历中修改导致迭代器失效
	auto copied = focusObjects;

	for (auto* it : copied)
	{
		if (it == nullptr)
		{
			std::cout << "what the hell" << std::endl;
			std::abort();
		}
		it->Unfocus(); // Unfocus 内部会修改原 focusObjects
	}
}



const std::unordered_set<Object*>& Scene::getFoucusedObjects() const
{
	return focusObjects;
}

void Scene::pickFocusObjectByRay(Ray ray)
{
	auto hitObjects = pickObjectsByRay(ray);

	if (hitObjects.empty())
	{
		preHitObjects.clear();
		clearFocusObj();
		return;
	}

	// 如果当前无选中对象 或者 多个被选中，默认选中新的第一个
	if (focusObjects.empty() || focusObjects.size() > 1)
	{
		clearFocusObj();
		hitObjects.front()->Focus();
	}
	else
	{
		float angle = GetAngle(preRay.rayDir, ray.rayDir);

		if (angle > 0.1f)
		{
			// 射线方向发生较大变化，重置选择
			clearFocusObj();
			hitObjects.front()->Focus();
		}
		else
		{
			// 射线方向基本没变，尝试切换到下一个被击中对象
			Object* currentFocus = *focusObjects.begin(); // 当前唯一 focus 的对象
			auto it = std::find(preHitObjects.begin(), preHitObjects.end(), currentFocus);

			clearFocusObj();

			if (it != preHitObjects.end())
			{
				++it;
				if (it != preHitObjects.end())
				{
					(*it)->Focus(); // 选中下一个
				}
				else
				{
					// 已是最后一个了，回到第一个
					preHitObjects.front()->Focus();
				}
			}
			else
			{
				// 当前 focus 对象不在 preHitObjects 中，直接选中第一个
				preHitObjects.front()->Focus();
			}
		}
	}

	// 更新记录
	preRay = ray;
	preHitObjects = std::move(hitObjects);
}



std::vector<Object*> Scene::pickObjectsByRay(Ray ray) const
{
	// 存储对象及其距离
	std::vector<std::pair<float, Object*>> candidates;

	for (auto& it : objectList)
	{
		if (!it->checkAABB) continue;

		auto* model = it->GetComponentAs<ModelComponent>();
		if (model == nullptr) continue;

		float near, far;
		if (model->aabb.intersectsRay(ray, near, far))
		{
			candidates.emplace_back(near, it);
		}
	}

	// 按照 near 距离排序
	std::sort(candidates.begin(), candidates.end(),
		[](const auto& a, const auto& b) {
			return a.first < b.first;
		});

	// 提取排序后的 Object*
	std::vector<Object*> ret;
	for (const auto& pair : candidates)
	{
		ret.emplace_back(pair.second);
	}

	return ret;
}




void Scene::saveJson()
{
	nlohmann::json js=nlohmann::json::array();
	for (auto& it:objects)
	{
		auto ptr = it.second.get();
		if (!ptr->NeedSave())continue;
		js.push_back(ptr->toJson());
	}
	std::string path = "C:\\Users\\Drwin\\Desktop\\render\\render3\\vs\\OpenGl\\Assests\\Resource\\ObjectData\\Scene.json";

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

void Scene::loadJson(const std::string& path)
{
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
	objectIndex = 0;

	for (const auto& objJson : js) {
		// 这里假设Object有静态函数fromJson返回Object指针或unique_ptr
		createObjectByJson(objJson);

	}
}


void Scene::loadJson()
{
	std::string path = "C:\\Users\\Drwin\\Desktop\\render\\render3\\vs\\OpenGl\\Assests\\Resource\\ObjectData\\Scene.json";
	loadJson(path);
}




