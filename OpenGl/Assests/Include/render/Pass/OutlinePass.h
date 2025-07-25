#pragma once
#include "baseShader.h"
#include "ModelComponent.h"
#include "RenderPass.h"



class OutlinePass:public RenderPass
{
public:
    OutlinePass();
    void getOutline();
    void execute(RenderContext& context) override;
	const  std::string& getName()override
    {
        static std::string name = "OutlinePass";
        return name;
    }
    
    void showUI() override;


    RendererTarget* finalOutlineRT()const { return outlineRT.get(); }
   
private:
    std::unique_ptr<baseShader> outlineShader;
    std::unique_ptr<baseShader> quadOutlineShader;
    std::unique_ptr<RendererTarget> outlineRT;
};
