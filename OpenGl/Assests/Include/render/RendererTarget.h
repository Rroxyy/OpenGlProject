#pragma once
#include <cstddef>
#include <iostream>
#include <glad/glad.h>

class RendererTarget
{
public:
    RendererTarget(int _width, int _height)
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
    }

    RendererTarget();
	/*:RendererTarget(
        GodClass::getInstance().getWidth(),
        GodClass::getInstance().getHeight())
    {}*/

    ~RendererTarget()
    {
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &rt_Id);
        glDeleteRenderbuffers(1, &rbo);
    }

    // 开始 渲染
    void begin();


    void resize(int newWidth, int newHeight)
    {
        if (newWidth == width && newHeight == height)
            return;  // 尺寸没变，直接返回

        width = newWidth;
        height = newHeight;

        // 绑定 FBO
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // 重新绑定并分配纹理内存
        glBindTexture(GL_TEXTURE_2D, rt_Id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        // 重新绑定并分配渲染缓冲内存（深度+模板）
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        // 重新附加纹理和渲染缓冲到FBO（可选，但写一下更保险）
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt_Id, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        // 检查完整性
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "FBO resize 后不完整!" << std::endl;

        // 解绑 FBO
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    // 结束 mask 渲染，恢复主屏幕
    void end()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    // 获取生成的 mask 纹理
    GLuint getRenderTextureId() const { return rt_Id; }
    GLuint getFBO() const { return fbo; }
    float getWidth()const  { return  width; }
    float getHeight() const { return height; }

private:
    GLuint fbo = 0;
    GLuint rt_Id = 0;
    GLuint rbo = 0;  // 深度缓冲

    int width = 0;
    int height = 0;
};
