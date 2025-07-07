#pragma once
#include <memory>
#include <unordered_map>


class Object;

class ObjectManager
{
public:
	static ObjectManager& getInstance()
	{
		static ObjectManager instance;
		return instance;
	}

	size_t addObject(Object* object);

	Object* getObject(size_t id);

	bool removeObject(size_t id);

	bool removeObject(Object* object);

	void saveJson();
private:
	size_t objectCnt=0;
	std::unordered_map<size_t, std::unique_ptr<Object>>objects;



	ObjectManager() = default;
	~ObjectManager() = default;

	ObjectManager(ObjectManager&) = delete;
	ObjectManager& operator=(ObjectManager&) = delete;

	ObjectManager(ObjectManager&&) = delete;
	ObjectManager& operator=(const ObjectManager&&) = delete;


};
