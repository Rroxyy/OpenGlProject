#include "ForwardPass.h"
#include "OutlinePass.h"
#include "Post-ProcessingPass.h"
#include "RenderPipeline.h"
#include "ShaderPathManager.h"


RenderPipeline::RenderPipeline()
{
	context = std::make_unique<RenderContext>();
	

	passes.emplace_back(std::make_unique<ForwardPass>());
	passesMap[typeid(ForwardPass).name()] = passes.back().get();

	passes.emplace_back(std::make_unique<OutlinePass>());
	passesMap[typeid(OutlinePass).name()] = passes.back().get();


	passes.emplace_back(std::make_unique<Post_ProcessingPass>());
	passesMap[typeid(Post_ProcessingPass).name()] = passes.back().get();
}

void RenderPipeline::start()
{
	for (auto& it : passes)
	{
		it.get()->start(*context.get());
	}
}



void RenderPipeline::renderScene()
{
	for (auto& it:passes)
	{
		it.get()->execute(*context.get());
	}
}

