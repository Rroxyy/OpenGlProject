#pragma once



class Component
{
public:
	std::string componentName;
	virtual const std::string& getComponentName() const = 0;
	virtual void loadJson(const nlohmann::json& js) = 0;
	virtual nlohmann::json toJson() = 0;
	virtual std::unique_ptr<Component> clone() const = 0;
	virtual void showUI() = 0;

private:
};
