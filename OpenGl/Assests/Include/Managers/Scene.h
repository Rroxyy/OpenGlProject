#pragma once
#include <json.hpp>
#include <memory>
#include <unordered_map>


class Object;


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

	const std::vector<Object*>& getObjectList()const { return objectList; }

	void saveJson();

	void loadJson();

	void start();
	void beforeUpdate();
	void update();
	void afterUpdate();

private:
	size_t objectCnt=0;
	std::unordered_map<size_t, std::unique_ptr<Object>>objects;
	std::vector<Object*> objectList;

	Scene() = default;
	~Scene();

	Scene(Scene&) = delete;
	Scene& operator=(Scene&) = delete;

	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&&) = delete;


};
