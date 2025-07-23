#pragma once
#include <memory>

#include "RenderPass.h"
#include "RendererTarget.h"
#include "UI_Manager.h"

class ForwardPass : public RenderPass {
public:
    void execute(RenderContext& context) override;
    const std::string& getName() override
    {
        static std::string name = "ForwardPass";
        return name;
    }


};
