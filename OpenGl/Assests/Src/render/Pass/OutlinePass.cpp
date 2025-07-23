#include "OutlinePass.h"

#include "GodClass.h"
#include "model.h"
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

    quadModel = std::make_unique<Model>();
    QuadMesh quadMesh;
    quadModel.get()->addMesh(&quadMesh);


   postProcessTarget = std::make_unique<RendererTarget>();


}


void OutlinePass::execute(RenderContext& context)
{
    outlineShader.get()->start();

    renderTarget->resize(GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight());
    renderTarget->begin();

    glClearColor(0,0,0,0);
    context.setRenderState(outlineShader.get());

    for (auto* obj : Scene::getInstance().getFoucusedObjects()) {
        Render* rd = obj->GetComponentExact<Render>();
        if (rd == nullptr)continue;
        Model* model = rd->getModel();


        outlineShader.get()->use(obj);
        model->Draw();
        outlineShader.get()->unuse();
    }
    renderTarget->end();


    postProcessTarget->resize(GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight());
    postProcessTarget->begin();  // 替代默认帧缓冲

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    quadOutlineShader->start();
    quadOutlineShader->use();

    // 输入为前面纯色的 renderTarget 的贴图
    GLuint colorTex = renderTarget->getRenderTextureId();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    quadOutlineShader->setInt("sceneColor", 0);

    quadOutlineShader->setVec2("texelSize", glm::vec2(
        1.0f / GodClass::getInstance().getWidth(),
        1.0f / GodClass::getInstance().getHeight()
    ));

    quadModel->Draw();  // 绘制后处理结果
    quadOutlineShader->unuse();

    postProcessTarget->end();  // 后处理结束

}


void OutlinePass::showUI()
{
    if (ImGui::TreeNode(getName().c_str()))
    {
        ImVec2 size(GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight());
        float scale = 512.0f / std::max(size.x, size.y);
        size.x *= scale;
        size.y *= scale;

        ImGui::Image(
            (void*)(intptr_t)renderTarget.get()->getRenderTextureId(),
            size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
        ImGui::Image(
            (void*)(intptr_t)postProcessTarget.get()->getRenderTextureId(),
            size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::TreePop();
    }
}

