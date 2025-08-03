#include "TextureResource.h"
#include "TextureManager.h"
#include <filesystem>


TextureResource::TextureResource()
{
	
}

TextureResource::TextureResource(const std::string& _filePath)
{
    if (!std::filesystem::exists(_filePath)) {
        std::cerr << "纹理文件不存在: " << _filePath << std::endl;
        std::abort();
    }
    filePath = _filePath;

    glGenTextures(1, &gltexture_id);
    SetTexture( );
}

TextureResource::TextureResource( size_t gl_id)
{
    gltexture_id = static_cast<unsigned int>(gl_id);
}

void TextureResource::SetTexture()
{
    auto data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    format = loadTextureFormat(nrChannels);

    if (data == nullptr)
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        std::abort();
    }
   
    glBindTexture(GL_TEXTURE_2D, gltexture_id);

    GLenum wrap = getGlWrapMode(wrapMode);

    GLenum minF = getGlFilterMode(minFilter);
    GLenum mag = getGlFilterMode(magFilter);

    // 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrap);
    // 过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minF);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
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
        return GL_NONE;  
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
    case FilterMode::Linear: return GL_LINEAR;
    case FilterMode::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
    case FilterMode::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
    case FilterMode::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
    case FilterMode::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
    default: return GL_LINEAR; // 默认值
    }
}

GLenum TextureResource::toGLTextureUnit(TextureChannel channel)
{
    // GL_TEXTURE0 是 0x84C0
    return static_cast<GLenum>(GL_TEXTURE0 + static_cast<int>(channel));
}


nlohmann::json TextureResource::toJson()
{
    nlohmann::json data;

    data["filePath"] = filePath;
    data["useMipmap"] = useMipmap;

    data["wrapMode"] = static_cast<int>(wrapMode);
    data["magFilter"] = static_cast<int>(magFilter);
    data["minFilter"] = static_cast<int>(minFilter);


    return data;
}


void TextureResource::loadJson(const nlohmann::json& data)
{
    filePath = data["filePath"].get<std::string>();

    std::cout << filePath << std::endl;

    useMipmap = data["useMipmap"].get<bool>();
    wrapMode = static_cast<WrapMode>(data["wrapMode"].get<int>());
    magFilter = static_cast<FilterMode>(data["magFilter"].get<int>());
    minFilter = static_cast<FilterMode>(data["minFilter"].get<int>());

    glGenTextures(1, &gltexture_id);
    SetTexture();

    generateMipmap();
    applyMagfilterSettings();
    applyTextureFilterSettings();
    applyWrapSettings();
}


