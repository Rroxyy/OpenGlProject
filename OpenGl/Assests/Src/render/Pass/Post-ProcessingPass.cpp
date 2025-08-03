#include "Post-ProcessingPass.h"

#include "ForwardPass.h"
#include "GodClass.h"
#include "OutlinePass.h"
#include "RenderPipeline.h"
#include "Scene.h"
#include "ShaderPathManager.h"
#include "TextureManager.h"

Post_ProcessingPass::Post_ProcessingPass()
{
    ppShader = std::make_unique<baseShader>(
        ShaderPathManager::getInstance().getQuadShaderVert().c_str(),
        "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/Post-ProcessShader/PP.frag",
		"Post Process Shader"
    );

    renderTarget->setClearColor(glm::vec4(0));


    finalRT = std::make_unique<RendererTarget>(0);
    finalShader = std::make_unique<baseShader>(
        ShaderPathManager::getInstance().getQuadShaderVert().c_str(),
        "C:/Users/Drwin/Desktop/render/render3/vs/OpenGl/Assests/Shaders/ShadersForClass/FinalPipelineShader/FinalOut.frag",
        "Final Out Shader"

    );

}

void Post_ProcessingPass::start(RenderContext& context)
{
    auto* colorTex = RenderPipeline::getInstance().passesMap[typeid(ForwardPass).name()]->getRendererTarget()->getTextureResource();
    auto* rawOutlineTex = RenderPipeline::getInstance().passesMap[typeid(OutlinePass).name()]->getRendererTarget()->getTextureResource();
    auto* dilatedTex = static_cast<OutlinePass*>(RenderPipeline::getInstance().passesMap[typeid(OutlinePass).name()])->getDilatedRT()->getTextureResource();


    ppShader.get()->setTexture("colorTex", colorTex);
    ppShader.get()->setTexture("rawOutlineTex", rawOutlineTex);
    ppShader.get()->setTexture("dilatedTex", dilatedTex);

    finalShader.get()->setTexture("finalTex", renderTarget->getTextureResource());
}


void Post_ProcessingPass::execute(RenderContext& context)
{
    auto* dilatedTex = static_cast<OutlinePass*>(RenderPipeline::getInstance().passesMap[typeid(OutlinePass).name()])->getDilatedRT()->getTextureResource();
    ppShader->changeTexture("dilatedTex", dilatedTex);

    renderTarget->resize();
    renderTarget->begin();
    ppShader->start();
    ppShader->use();
    ModelManager::getInstance().getQuadMesh()->Draw();
    ppShader->unuse();
    renderTarget->end();
    outlinePreFrame = true;
    
   


    finalRT.get()->resize();
    finalRT.get()->begin();
    finalShader->start();
    finalShader->use();
    ModelManager::getInstance().getQuadMesh()->Draw();
    finalShader->unuse();
    finalRT.get()->end();
}


void Post_ProcessingPass::showUI()
{
    if (ImGui::TreeNode(getName().c_str()))
    {
        ImVec2 size=ImVec2(GodClass::getInstance().getScale().y, GodClass::getInstance().getScale().x);
        float scale = 512.0f / std::max(size.x,size.y);
        size.x *= scale;
        size.y *= scale;


        ppShader->showUI();
        ImGui::Spacing();

        ImGui::Image(
            (void*)(intptr_t)renderTarget.get()->getRenderTextureId(),
            size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );


        
        
      

        ImGui::TreePop();
    }
}
