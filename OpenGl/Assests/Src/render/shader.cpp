#include "Shader.h"
#include "TextureResource.h"


void Shader::useTexture(std::string&& nameInShader,TextureResource& tr)
{
    setInt(nameInShader, tr.getTextureUnitIndex());
}