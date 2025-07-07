#pragma once



class Component
{
public:
	std::string componentName;
	virtual const std::string& getComponentName() const = 0;
	virtual nlohmann::json toJson() = 0;


private:
};
