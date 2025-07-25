#include "RenderPass.h"

#include "GodClass.h"
#include "UI_Manager.h"


RenderPass::RenderPass()
{
    auto showUIFunc = [this]()->void
        {
            showUI();
        };

    UIManager::getInstance().RegisterForRendererPassesWindow(showUIFunc);
    renderTarget = std::make_unique<RendererTarget>();
}

void RenderPass::showUI()
{
    if (ImGui::TreeNode(getName().c_str()))
    {
        ImVec2 size(static_cast<float>(GodClass::getInstance().getWidth()), static_cast<float>(GodClass::getInstance().getHeight()));
        float scale = 512.0f / std::max(size.x, size.y);
        size.x *= scale;
        size.y *= scale;

        ImGui::Image(
            (void*)(intptr_t)renderTarget.get()->getRenderTextureId(),
            size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

       
        ImGui::TreePop();
    }
}
