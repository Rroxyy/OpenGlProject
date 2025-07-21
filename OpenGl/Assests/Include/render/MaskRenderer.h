#pragma once
#include <cstddef>
#include <iostream>
#include <glad/glad.h>

#include "GodClass.h"

class MaskRenderer
{
public:
    MaskRenderer()
    {
        width = GodClass::getInstance().getWidth();
        height = GodClass::getInstance().getHeight();

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // 创建 mask 纹理（单通道）
        glGenTextures(1, &maskTex);
        glBindTexture(GL_TEXTURE_2D, maskTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // 绑定到 FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, maskTex, 0);

        // 不需要深度贴图的情况可以省略，但一般推荐配一个深度 buffer：
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        // 检查 FBO 完整性
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Mask FBO 创建失败" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~MaskRenderer()
    {
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &maskTex);
        glDeleteRenderbuffers(1, &rbo);
    }

    // 开始 mask 渲染
    void begin()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 黑色为默认未选中区域
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }

    // 结束 mask 渲染，恢复主屏幕
    void end()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // 获取生成的 mask 纹理
    GLuint getMaskTexture() const { return maskTex; }

private:
    GLuint fbo = 0;
    GLuint maskTex = 0;
    GLuint rbo = 0;  // 深度缓冲

    int width = 0;
    int height = 0;
};
