#pragma once
#include <imgui.h>

#include "Component.h"
#include "ModelComponent.h"


class baseShader;
class ModelComponent;

class Render:public Component
{
public:
	Render()
	{
		componentName = "Render";
	}
	const std::string getComponentName() const override
	{
		return componentName;

	}

	std::unique_ptr<Component> clone() const override
	{
		return std::make_unique<Render>(*this);
	}

	void showUI() override
	{
		if (ImGui::TreeNode(getComponentName().c_str()))
		{
			ImGui::Spacing();
			ImGui::Text("Render Info:");
			ImGui::Separator();

			ImGui::Text("  ModelComponent  : %s", model->getModelPath().size()==0?"null": model->getModelPath());
			ImGui::Text("  Shader : %s", shader->getShaderName().c_str());

			ImGui::Spacing();
			ImGui::TreePop();
		}
	}

	nlohmann::json toJson() override
	{
		nlohmann::json ret;
		nlohmann::json data;

		ret["componentName"] = getComponentName();
		ret["componentData"] = data;

		return ret;
	}

	void loadJson(const nlohmann::json& js) override
	{
		;
	}


	void start() override
	{
		if (object==nullptr)
		{
			std::cout << "Warnning: no object has set" << std::endl;
			return;
		}

		model=object->GetComponentExact<ModelComponent>();
		shader=object->GetComponentAs<baseShader>();
	}

	/*void beFocused()
	{
		RendererTarget mr;
		mr.begin();
		GodClass::getInstance().getFocusShader()->use();
		model->Draw();
		GodClass::getInstance().getFocusShader()->unuse();
		mr.end();
	}*/


	ModelComponent* getModel()const { return model; }
	baseShader* getShader()const { return shader; }

private:
	ModelComponent* model;
	baseShader* shader;

};
