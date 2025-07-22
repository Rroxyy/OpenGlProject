#pragma once
#include <memory>

#include "RenderPass.h"
#include "RendererTarget.h"
#include "UI_Manager.h"

class ForwardPass : public RenderPass {
public:
    ForwardPass()
    {
        auto showUIFunc = [this]()->void
            {
                showUI();
            };

        UIManager::getInstance().Register(showUIFunc);
        renderTarget = std::make_unique<RendererTarget>();
    }

    void execute(RenderContext& context) override;


    const std::string& getName() const override {
        static std::string name = "ForwardPass";
        return name;
    }

    GLuint getColorTexture() const {
        return renderTarget->getRenderTextureId();  // 用于后处理、屏幕绘制等
    }

    void showUI()
    {
	    if (ImGui::TreeNode(getName().c_str()))
	    {
            ImGui::Begin("RT Debug");
            ImVec2 size(GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight());
            float scale = 512.0f / std::max(size.x, size.y);
            size.x *= scale;
            size.y *= scale;

            ImGui::Image(
                (void*)(intptr_t)getColorTexture(),
                size,
                ImVec2(0, 1),
                ImVec2(1, 0)
            );

            ImGui::End();

            ImGui::TreePop();
	    }
    }

private:
   
    std::unique_ptr<RendererTarget> renderTarget;
};
