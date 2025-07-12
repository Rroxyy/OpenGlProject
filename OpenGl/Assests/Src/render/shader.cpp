#include "Shader.h"
#include "TextureResource.h"


void Shader::blindTextureToShader(const std::string& nameInShader,int shaderChannel)const
{
    setInt(nameInShader, shaderChannel);
}