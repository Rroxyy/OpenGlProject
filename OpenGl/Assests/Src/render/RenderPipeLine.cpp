#include "ForwardPass.h"
#include "OutlinePass.h"
#include "RenderPipeline.h"


RenderPipeline::RenderPipeline()
{
	context = std::make_unique<RenderContext>();

	passes.emplace_back(std::make_unique<ForwardPass>());
	passesMap[typeid(ForwardPass).name()] = passes.back().get();

	passes.emplace_back(std::make_unique<OutlinePass>());
	passesMap[typeid(OutlinePass).name()] = passes.back().get();



}


void RenderPipeline::renderScene()
{
	for (auto& it:passes)
	{
		it.get()->execute(*context.get());
	}

	ForwardPass* fp = static_cast<ForwardPass*>(passesMap[typeid(ForwardPass).name()]);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fp->getRendererTarget()->getFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  // 默认 framebuffer
	glBlitFramebuffer(0, 0, fp->getRendererTarget()->getWidth(), fp->getRendererTarget()->getHeight(),
		0, 0, GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight(),
		GL_COLOR_BUFFER_BIT, GL_NEAREST);


}

