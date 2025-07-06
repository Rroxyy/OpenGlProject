#pragma once



class Component
{
public:
	virtual ~Component() {}
	virtual void Update(float dt) {}

private:
};
