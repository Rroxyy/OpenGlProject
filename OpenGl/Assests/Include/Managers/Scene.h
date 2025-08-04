#pragma once
#include <json.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Object.h"
#include "Ray.h"


class Ray;



//object manager
class Scene
{
public:
	static Scene& getInstance()
	{
		static Scene instance;
		return instance;
	}

	size_t createObject();

	Object* createObjectPtr();

	Object* createObjectPtr(std::string&&name);

	void createObjectByJson(const nlohmann::json& js);

	Object* getObject(size_t id);

	bool removeObject(size_t id);
	bool removeObject(Object* object);

	void addFocusedObj(Object* obj);
	void removeFocusedObj(Object* obj);
	void clearFocusObj();

	const std::unordered_set<Object*>& getFoucusedObjects()const;
	const std::vector<Object*>& getObjectList()const { return objectList; }

	//AABB
	void pickFocusObjectByRay(Ray ray);
	std::vector<Object*> pickObjectsByRay(Ray ray)const;

	void saveJson();

	void loadJson();
	void loadJson(const std::string& path);

	void start();
	void beforeUpdate();
	void update();
	void afterUpdate();

private:
	size_t objectIndex=0;
	std::unordered_map<size_t, std::unique_ptr<Object>>objects;
	std::vector<Object*> objectList;
	std::unordered_set<Object*>focusObjects;

	//AABB
	Ray preRay;
	std::vector<Object*> preHitObjects;

	Scene() = default;
	~Scene();

	Scene(Scene&) = delete;
	Scene& operator=(Scene&) = delete;

	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&&) = delete;


};
