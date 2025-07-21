#pragma once
#include "RenderPass.h"
#include "RenderContext.h"


class ForwardPass : public RenderPass {
public:
    void execute(RenderContext& context) override;


    const std::string& getName() const override {
        static std::string name = "ForwardPass";
        return name;
    }

   

   

private:
    
};
