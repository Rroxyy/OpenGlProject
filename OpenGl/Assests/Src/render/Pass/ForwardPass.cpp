#include "ForwardPass.h"

#include "Render.h"
#include "Scene.h"
#include "RenderContext.h"



void ForwardPass::execute(RenderContext& context)
{
    renderTarget->begin();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // 红色

    for (auto* obj : Scene::getInstance().getObjectList()) {
        auto* render = obj->GetComponentExact<Render>();
        if (!render)continue;

        baseShader* shader = render->getShader();
        Model* model = render->getModel();

        context.setRenderState(shader->useDepth, shader->cullMode);

        shader->use(context);
        model->Draw();
        shader->unuse();
    }
    renderTarget->end();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderTarget->getFBO());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  // 默认 framebuffer
    glBlitFramebuffer(0, 0, renderTarget->getWidth(),renderTarget->getHeight(),
        0, 0, GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight(),
        GL_COLOR_BUFFER_BIT, GL_NEAREST);

}


