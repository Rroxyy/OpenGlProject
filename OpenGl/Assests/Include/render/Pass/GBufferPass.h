//#pragma once
//#include "RenderContext.h"
//#include "RenderPass.h"
//
//
//class GBufferPass : public RenderPass {
//public:
//    void initialize() override {
//        // 创建 MRT Framebuffer + Shader
//      /*  framebuffer = createFramebufferWithMRT(...);
//        gbufferShader = Shader::load("gbuffer.vert", "gbuffer.frag");*/
//    }
//
//    void execute(RenderContext& context) override {
//      /*  context.bindFramebuffer(framebuffer);
//        context.clear(...);
//
//        gbufferShader->bind();
//
//        for (auto& obj : visibleOpaqueObjects) {
//            obj->bindMaterial(gbufferShader);
//            obj->draw(context);
//        }*/
//    }
//
//    const std::string& getName() const override {
//        static std::string name = "GBufferPass";
//        return name;
//    }
//
//private:
//    Framebuffer* framebuffer;
//    Shader* gbufferShader;
//    std::vector<RenderObject*> visibleOpaqueObjects;
//};
