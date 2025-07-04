#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "TextureEnums.h"

class TextureResource;
class TextureManager {
public:
     static TextureManager& getInstance();
    ~TextureManager();

    

    TextureChannel getTexChannel(TextureResource* tr);
    

private:
    unsigned int nowIndex;
    std::unordered_map<TextureResource*, TextureChannel> m;
    TextureManager();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
};



#endif
