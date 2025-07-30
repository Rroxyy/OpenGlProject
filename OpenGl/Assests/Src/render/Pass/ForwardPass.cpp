#include "ForwardPass.h"

#include "Render.h"
#include "Scene.h"
#include "RenderContext.h"


//run per frame
void ForwardPass::execute(RenderContext& context)
{
    renderTarget->resize(GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight());
    renderTarget->begin();

    for (auto* obj : Scene::getInstance().getObjectList()) {
        auto* render = obj->GetComponentExact<Render>();
        if (!render)continue;

        baseShader* shader = render->getShader();
        ModelComponent* model = render->getModel();
        context.setRenderState(shader);
        shader->use();
        model->Draw();
        shader->unuse();
    }
    renderTarget->end();
}


void ForwardPass::showUI()
{
    if (ImGui::TreeNode(getName().c_str()))
    {
        ImVec2 size(static_cast<float>(GodClass::getInstance().getWidth()), static_cast<float>(GodClass::getInstance().getHeight()));
        float scale = 512.0f / std::max(size.x, size.y);
        size.x *= scale;
        size.y *= scale;


        ImGui::ColorPicker4("Sky Color", (float*)&skyColor);
        renderTarget->setClearColor(skyColor);
        ImGui::Spacing();

        ImGui::Image(
            (void*)(intptr_t)renderTarget.get()->getRenderTextureId(),
            size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );


        ImGui::TreePop();
    }
}





