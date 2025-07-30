#include "RendererTarget.h"

#include "GodClass.h"
#include "TextureManager.h"

RendererTarget::RendererTarget(int _width, int _height)
{
    width = _width;
    height = _height;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &rt_Id);
    glBindTexture(GL_TEXTURE_2D, rt_Id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 绑定到 FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt_Id, 0);

    // 不需要深度贴图的情况可以省略，但一般推荐配一个深度 buffer：
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // 检查 FBO 完整性
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Mask FBO 创建失败" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    texture_resource = TextureManager::getInstance().createTextureResourceByGlid(rt_Id);


    
}


RendererTarget::RendererTarget() :RendererTarget(
    GodClass::getInstance().getWidth(),
    GodClass::getInstance().getHeight())
{
	
}

void RendererTarget::resize()
{
    resize(GodClass::getInstance().getWidth(), GodClass::getInstance().getHeight());
}



void RendererTarget::begin()
{

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
    glClearColor(defaultColor.x, defaultColor.y, defaultColor.z, defaultColor.w); // [1] 先设置颜色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                   // [2] 再清除缓冲
    glEnable(GL_DEPTH_TEST);

}


