#include "ForwardPass.h"

#include "Render.h"
#include "Scene.h"
#include "RenderContext.h"


//run per frame
void ForwardPass::execute(RenderContext& context)
{
    renderTarget->resize(GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight());
    renderTarget->begin();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 

    for (auto* obj : Scene::getInstance().getObjectList()) {
        auto* render = obj->GetComponentExact<Render>();
        if (!render)continue;

        baseShader* shader = render->getShader();
        Model* model = render->getModel();
        context.setRenderState(shader);
        shader->use();
        model->Draw();
        shader->unuse();
    }
    renderTarget->end();
}





