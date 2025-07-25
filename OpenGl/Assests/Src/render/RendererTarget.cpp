#include "RendererTarget.h"

#include "GodClass.h"

RendererTarget::RendererTarget() :RendererTarget(
    GodClass::getInstance().getWidth(),
    GodClass::getInstance().getHeight())
{
	
}



void RendererTarget::begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}
