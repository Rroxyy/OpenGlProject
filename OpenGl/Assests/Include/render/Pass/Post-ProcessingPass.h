#pragma once
#include "baseShader.h"
#include "RenderPass.h"

class Post_ProcessingPass:public RenderPass
{
public:
	Post_ProcessingPass();
	void start(RenderContext& context) override;
	void execute(RenderContext& context) override;
	const std::string& getName() override
	{
		static std::string name = "Post Process Pass";
		return name;
	}

	void showUI() override;
private:
	std::unique_ptr<baseShader> ppShader;

	std::unique_ptr<RendererTarget> finalRT;
	std::unique_ptr<baseShader>finalShader;
	
	
	bool outlinePreFrame = false;

};
