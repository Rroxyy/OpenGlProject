#pragma once
#include <memory>

#include "RenderPass.h"
#include "RendererTarget.h"
#include "UI_Manager.h"

class ForwardPass : public RenderPass {
public:
    ForwardPass()
    {
        renderTarget->setClearColor(skyColor);
        renderTarget->setDepthBuffer(true);
    }
    void execute(RenderContext& context) override;
    const std::string& getName() override
    {
        static std::string name = "ForwardPass";
        return name;
    }

    void showUI() override;
private:
    glm::vec4 skyColor = glm::vec4(0.1, 0.1, 0.1, 1);

};
