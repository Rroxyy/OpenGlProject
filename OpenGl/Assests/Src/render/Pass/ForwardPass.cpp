#include "ForwardPass.h"

#include "Render.h"
#include "Scene.h"

//#pragma message("Compiling ForwardPass.cpp")


void ForwardPass::execute(RenderContext& context)
{
    context.clear(0.1f, 0.1f, 0.1f, 1.0f, true);  // 清除颜色 + 深度

    for (auto* obj : Scene::getInstance().getObjectList()) {
        auto* render = obj->GetComponentExact<Render>();
        if (!render)continue;

        baseShader* shader = render->getShader();
        Model* model = render->getModel();


        shader->use(context);
        model->Draw();
        shader->unuse();
    }
}
