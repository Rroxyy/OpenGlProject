#pragma once
#include "RenderContext.h"
#include "RenderPass.h"


class RenderContext;


class RenderPipeline
{
public:
    static RenderPipeline& getInstance()
    {
        static RenderPipeline instance;
        return instance;
    }

    RenderContext* getRenderContext()
    {
        return context.get();
    }
    
    void renderScene();

private:
    std::unique_ptr<RenderContext> context;
    std::vector<std::unique_ptr<RenderPass>> passes;
    std::unordered_map<std::string, RenderPass*> passesMap;


    RenderPipeline();
    ~RenderPipeline() {}
    RenderPipeline(const RenderPipeline&) = delete;
    void operator=(const RenderPipeline&) = delete;
};
