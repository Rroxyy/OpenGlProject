#pragma once


#include <string>
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>

#include "TextureEnums.h"


class Shader;

class TextureResource
{
public:
	friend class TextureManager;


	void SetTexture( WrapMode wrap_mode, FilterMode filter_mode, bool useMipmap);

	void setToShader(Shader& shader);

	unsigned int getGLtexture_id()
	{
		return gltexture_id;
	}


private:
	size_t textureManger_id;
	unsigned int gltexture_id;
	std::string filePath;


	int height;
	int width;
	int nrChannels;
	int format;

	TextureResource(size_t id,const std::string& filePath);
	TextureResource(size_t gl_id);


	static GLenum loadTextureFormat(int& nrChannels);
	static GLenum getGlWrapMode(WrapMode mode);
	static GLenum getGlFilterMode(FilterMode mode);
	static GLenum toGLTextureUnit(TextureChannel channel);
};