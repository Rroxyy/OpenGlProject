#include "TextureResource.h"
#include "TextureManager.h"
#include <filesystem>



TextureResource::TextureResource(size_t id,const std::string& _filePath)
{
    if (!std::filesystem::exists(_filePath)) {
        std::cerr << "纹理文件不存在: " << _filePath << std::endl;
        std::abort();
    }
    filePath = _filePath;

    glGenTextures(1, &gltexture_id);
    SetTexture(WrapMode::Repeat, FilterMode::Linear, false);
}

TextureResource::TextureResource( size_t gl_id)
{
    gltexture_id = gl_id;
}

void TextureResource::SetTexture(WrapMode wrap_mode,FilterMode filter_mode,bool useMipmap)
{
    auto data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    format = loadTextureFormat(nrChannels);

    if (data == nullptr)
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        std::abort();
    }
   
    glBindTexture(GL_TEXTURE_2D, gltexture_id);

    GLenum wrap = getGlWrapMode(wrap_mode);
    GLenum filter = getGlFilterMode(filter_mode);

    // 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrap);
    // 过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    //生成mipmap
    if (useMipmap)glGenerateMipmap(GL_TEXTURE_2D);


    stbi_image_free(data);
}




//unfinished
void TextureResource::setToShader(Shader& shader)
{
	
}


GLenum TextureResource::loadTextureFormat(int& nrChannels)
{
    if (nrChannels == 1)
        return GL_RED;
    else if (nrChannels == 3)
        return GL_RGB;
    else if (nrChannels == 4)
        return GL_RGBA;
    else {
        std::cerr << "[Error] Unsupported texture channel count: " << nrChannels << std::endl;
        return GL_NONE;  // 或者 return GL_NONE;
    }
}

GLenum TextureResource::getGlWrapMode(WrapMode mode)
{
    switch (mode) {
    case WrapMode::Repeat:         return GL_REPEAT;
    case WrapMode::ClampToEdge:    return GL_CLAMP_TO_EDGE;
    case WrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
    default:
        std::cerr << "[Error] Unknown WrapMode passed to getGlWrapMode\n";
        return GL_REPEAT; // or GL_NONE
    }
}

GLenum TextureResource::getGlFilterMode(FilterMode mode)
{
    switch (mode) {
    case FilterMode::Nearest: return GL_NEAREST;
    case FilterMode::Linear:  return GL_LINEAR;
    default:
        std::cerr << "[Error] Unknown FilterMode passed to getGlFilterMode\n";
        return GL_LINEAR; // or GL_NONE
    }
}

GLenum TextureResource::toGLTextureUnit(TextureChannel channel)
{
    // GL_TEXTURE0 是 0x84C0
    return static_cast<GLenum>(GL_TEXTURE0 + static_cast<int>(channel));
}






