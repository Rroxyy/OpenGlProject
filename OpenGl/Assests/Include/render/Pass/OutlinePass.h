#pragma once
#include "baseShader.h"
#include "ModelComponent.h"
#include "RenderPass.h"



class OutlinePass:public RenderPass
{
public:
    OutlinePass();
    void dilatedRT();
    void execute(RenderContext& context) override;
	const  std::string& getName()override
    {
        static std::string name = "OutlinePass";
        return name;
    }
    void showUI() override;


    RendererTarget* getDilatedRT()const;
   
private:
    int outlineWidth = 3;

    std::unique_ptr<baseShader> outlineShader;
    std::unique_ptr<baseShader> quadOutlineShader;



    bool useRT1;
    std::unique_ptr<RendererTarget>rt1;
    std::unique_ptr<RendererTarget>rt2;


    bool hasOutline = false;
};
