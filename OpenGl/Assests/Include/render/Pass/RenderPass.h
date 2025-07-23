#pragma once
#include <string>

#include "RendererTarget.h"


class RenderContext;

class RenderPass {
public:
    RenderPass();
    
    virtual ~RenderPass() {}

    virtual void execute(RenderContext& context) = 0; // 每帧执行
    virtual const std::string& getName()
    {
        static std::string name = "RenderPass";
        return name;
    }
    RendererTarget* getRendererTarget()const
    {
        return renderTarget.get();
    }
    virtual void showUI();

protected:
    std::unique_ptr<RendererTarget> renderTarget;
};
