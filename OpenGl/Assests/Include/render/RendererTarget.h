#pragma once
#include <cstddef>
#include <iostream>
#include <glad/glad.h>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec4.hpp>

#include "TextureResource.h"

class RendererTarget
{
public:
    RendererTarget(int _width, int _height);
    RendererTarget();
    RendererTarget(GLuint existingTexId);
   


    ~RendererTarget()
    {
        if (dontRelease)return;
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &rt_Id);
        glDeleteRenderbuffers(1, &rbo);
    }

   
    void setClearColor(const glm::vec4& _color)
    {
        defaultColor = _color;
    }


    void setDepthBuffer(bool _useDepthBuffer)
    {
        if (useDepthBuffer ^ _useDepthBuffer)
        {
            useDepthBuffer = _useDepthBuffer;
            updateDepthBuffer();
        }
    }
    void updateDepthBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        if (useDepthBuffer)
        {
            // 如果之前有旧的 rbo，先删掉
            if (rbo)
            {
                glDeleteRenderbuffers(1, &rbo);
                rbo = 0;
            }

            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        }
        else
        {
            // 移除 FBO 上绑定的深度附件
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
            // 删除 renderbuffer 对象
            if (rbo)
            {
                glDeleteRenderbuffers(1, &rbo);
                rbo = 0;
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void setEmpty()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
        glClearColor(defaultColor.x, defaultColor.y, defaultColor.z, defaultColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    // 开始 渲染
    void begin();

    void resize();
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
        updateDepthBuffer();

        texture_resource->applyWrapSettings();
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
        texture_resource->generateMipmap();
    }

   


    // 获取生成的 mask 纹理
    GLuint getRenderTextureId() const { return rt_Id; }

    TextureResource* getTextureResource() { return texture_resource.get(); }


    GLuint getFBO() const { return fbo; }
    float getWidth() const { return static_cast<float>(width); }
    float getHeight() const { return static_cast<float>(height); }

private:
    GLuint fbo = 0;
    GLuint rt_Id = 0;
    GLuint rbo = 0;  // 深度缓冲
    std::unique_ptr<TextureResource> texture_resource;

    bool dontRelease = false;
    bool useDepthBuffer = false;

    glm::vec4 defaultColor = glm::vec4(0);

    int width = 0;
    int height = 0;
};
