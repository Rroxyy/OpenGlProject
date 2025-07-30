#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <memory>
#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "TextureEnums.h"

class TextureResource;
class TextureManager {
public:
     static TextureManager& getInstance();
    ~TextureManager();

    

    //TextureChannel getTexChannel(TextureResource* tr);
    
    TextureResource* createTextureResourceByPath(const std::string& _filePath);
    std::unique_ptr<TextureResource> createTextureResourceByGlid(size_t glTexture_id);

    TextureResource* getTextureByPath(const std::string& _filePath) const;
private:
    size_t nowIndex=0;
    //std::unordered_map<unsigned int, std::unique_ptr<TextureResource>>textureResources;
    std::unordered_map<size_t,std::unique_ptr<TextureResource>> textureResourcesMap;
    std::unordered_map<std::string, size_t>filePathMap;
    TextureManager();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
};



#endif
