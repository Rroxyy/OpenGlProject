#include "OutlinePass.h"

#include "GodClass.h"
#include "ModelComponent.h"
#include "QuadMesh.h"
#include "Render.h"
#include "Scene.h"
#include "RenderContext.h"
#include "ResourcePathManager.h"

OutlinePass::OutlinePass()
{
    outlineShader = std::make_unique<baseShader>(
		ResourcePathManager::getInstance().getOutlineShaderVert().c_str(),
        ResourcePathManager::getInstance().getOutlineShaderFrag().c_str(),
		"Outline Shader"
    );

    quadOutlineShader = std::make_unique<baseShader>(
        ResourcePathManager::getInstance().  getQuadOutlineShaderVert().c_str(),
        ResourcePathManager::getInstance().getQuadOutlineShaderFrag().c_str(),
        "Quad Outline Shader"
    );

   outlineRT = std::make_unique<RendererTarget>();
}




void OutlinePass::execute(RenderContext& context)
{
    if (Scene::getInstance().getFoucusedObjects().empty())return;


    outlineShader.get()->start();
  


    renderTarget->resize(GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight());
    renderTarget->begin();

   /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);*/

    glClearColor(0,0,0,0);
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
   /* glBindTexture(GL_TEXTURE_2D, renderTarget.get()->getRenderTextureId());
    glGenerateMipmap(GL_TEXTURE_2D);*/

    getOutline();

}

void OutlinePass::getOutline()
{
    outlineRT->resize(GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight());
    outlineRT->begin();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    quadOutlineShader->start();

    // 输入为前面纯色的 renderTarget 的贴图
    GLuint colorTex = renderTarget->getRenderTextureId();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    quadOutlineShader->use();
    quadOutlineShader->setInt("sceneTex", 0);
    quadOutlineShader->setVec2("texelSize", glm::vec2(
        1.0f / GodClass::getInstance().getWidth(),
        1.0f / GodClass::getInstance().getHeight()
    ));

    ResourceManager::getInstance().getQuadMesh()->Draw();  // 绘制后处理结果
    quadOutlineShader->unuse();

    outlineRT->end();  // 后处理结束
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

        ImGui::Image(
            (void*)(intptr_t)renderTarget.get()->getRenderTextureId(),
            size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
        ImGui::Image(
            (void*)(intptr_t)outlineRT.get()->getRenderTextureId(),
            size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::TreePop();
    }
}

