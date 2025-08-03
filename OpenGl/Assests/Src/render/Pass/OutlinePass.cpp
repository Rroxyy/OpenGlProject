#include "OutlinePass.h"

#include "GodClass.h"
#include "ModelComponent.h"
#include "QuadMesh.h"
#include "Render.h"
#include "Scene.h"
#include "RenderContext.h"
#include "ShaderPathManager.h"

OutlinePass::OutlinePass()
{
    outlineShader = std::make_unique<baseShader>(
		ShaderPathManager::getInstance().getOutlineShaderVert().c_str(),
        ShaderPathManager::getInstance().getOutlineShaderFrag().c_str(),
		"Outline Shader"
    );

    quadOutlineShader = std::make_unique<baseShader>(
        ShaderPathManager::getInstance().  getQuadOutlineShaderVert().c_str(),
        ShaderPathManager::getInstance().getQuadOutlineShaderFrag().c_str(),
        "Quad Outline Shader"
    );


   useRT1 = true;
   rt1 = std::make_unique<RendererTarget>();
   rt2 = std::make_unique<RendererTarget>();

   rt1->getTextureResource()->setWrapMode(WrapMode::ClampToEdge);
   rt2->getTextureResource()->setWrapMode(WrapMode::ClampToEdge);

}




void OutlinePass::execute(RenderContext& context)
{
    if (Scene::getInstance().getFoucusedObjects().empty())
    {
        hasOutline = false;
	    return;
    }
    outlineShader.get()->start();
    renderTarget->resize();
    renderTarget->begin();
    context.setRenderState(outlineShader.get());
    for (auto* obj : Scene::getInstance().getFoucusedObjects()) {
        Render* rd = obj->GetComponentExact<Render>();
        if (rd == nullptr)continue;
        ModelComponent* model = rd->getModel();
        outlineShader.get()->use(obj);
        model->Draw();
        outlineShader.get()->unuse();
    }
    renderTarget->end();
    dilatedRT();
    hasOutline = true;
}

void OutlinePass::dilatedRT()
{
    RendererTarget* preRT;
	RendererTarget* outRT;
    for (int i=1;i<=outlineWidth;i++)
    {
    	preRT= i == 1 ? renderTarget.get() : useRT1 ? rt1.get() : rt2.get();
    	outRT= useRT1 ? rt2.get() : rt1.get();
        useRT1 ^= 1;

        outRT->resize();
        outRT->begin();

        quadOutlineShader->start();

        // 输入为前面纯色的 renderTarget 的贴图
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, preRT->getRenderTextureId());

        
        quadOutlineShader->use();
        quadOutlineShader->setInt("sceneTex", 0);
        quadOutlineShader->setVec2("texelSize", glm::vec2(
            1.0f / GodClass::getInstance().getWidth(),
            1.0f / GodClass::getInstance().getHeight()
        ));

        ModelManager::getInstance().getQuadMesh()->Draw();  // 绘制后处理结果
        quadOutlineShader->unuse();

        outRT->end();
    }


}



RendererTarget* OutlinePass::getDilatedRT() const
{
    if (!hasOutline)
    {
        return GodClass::getInstance().getEmptyRenderTarget();
    }
	return useRT1 ? rt1.get() : rt2.get();
}



void OutlinePass::showUI()
{
    if (ImGui::TreeNode(getName().c_str()))
    {
        ImVec2 size(static_cast<float>(GodClass::getInstance().getWidth()), static_cast<float>(GodClass::getInstance().getHeight()));
        float scale = 512.0f / std::max(size.x, size.y);
        size.x *= scale;
        size.y *= scale;

        quadOutlineShader->showUI();

        ImGui::Spacing();
        ImGui::SliderInt("Outline Width", &outlineWidth, 1, 10);

        ImGui::Image(
            (void*)(intptr_t)renderTarget.get()->getRenderTextureId(),
            size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        RendererTarget* temp = useRT1 ? rt1.get() : rt2.get();
        ImGui::Image(
            (void*)(intptr_t)temp->getRenderTextureId(),
            size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::TreePop();
    }
}

