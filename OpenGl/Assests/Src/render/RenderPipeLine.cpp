#include "ForwardPass.h"
#include "GBufferPass.h"
#include "RenderPipeline.h"


RenderPipeline::RenderPipeline()
{
	context = std::make_unique<RenderContext>();

	passes.push_back(std::make_unique<ForwardPass>());

}


void RenderPipeline::renderScene()
{
	for (auto& it:passes)
	{
		it.get()->execute(*context.get());
	}
}

