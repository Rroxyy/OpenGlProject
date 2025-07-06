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
	char const* filePath;

	TextureResource(TextureChannel textureChannel, char const* filePath);
	TextureResource(char const* filePath);

	~TextureResource();

	void SetTexture( WrapMode wrap_mode, FilterMode filter_mode, bool useMipmap);

	void setToShader(Shader& shader);
	void activeTexture() const;

	GLenum getGLTextureUnit() const;
	int getTextureUnitIndex() const;
private:
	unsigned int texture_id;
	TextureChannel textureChannel;
	int height;
	int width;
	int nrChannels;
	int format;


	static GLenum loadTextureFormat(int& nrChannels);
	static GLenum getGlWrapMode(WrapMode mode);
	static GLenum getGlFilterMode(FilterMode mode);
	static GLenum toGLTextureUnit(TextureChannel channel);
};