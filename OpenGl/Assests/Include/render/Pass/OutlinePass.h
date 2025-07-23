#pragma once
#include "baseShader.h"
#include "model.h"
#include "RenderPass.h"


class OutlinePass:public RenderPass
{
public:
    OutlinePass();
	void execute(RenderContext& context) override;
	const  std::string& getName()override
    {
        static std::string name = "OutlinePass";
        return name;
    }
    
    void showUI() override;
   
private:
    std::unique_ptr<baseShader> outlineShader;
    std::unique_ptr<baseShader> quadOutlineShader;

    std::unique_ptr<Model> quadModel;

    std::unique_ptr<RendererTarget> postProcessTarget;
};
