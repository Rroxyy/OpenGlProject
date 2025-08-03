#pragma once


#include <string>
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include <json.hpp>

#include "TextureEnums.h"


class Shader;

class TextureResource
{
public:
	friend class TextureManager;
	void SetTexture();
	unsigned int getGLtexture_id()
	{
		return gltexture_id;
	}

	//Mipmap Settings
	void setMipmap(bool _useMipmap,FilterMode _mode=FilterMode::LinearMipmapLinear)
	{

		if (_useMipmap^useMipmap||minFilter!=_mode)
		{
			useMipmap = _useMipmap;
			minFilter = _mode;

			applyTextureFilterSettings();
		}
	}
	void applyTextureFilterSettings()
	{
		GLenum filter = getGlFilterMode(minFilter);
		GLenum mag = getGlFilterMode(magFilter);

		glBindTexture(GL_TEXTURE_2D, gltexture_id);

		if (useMipmap)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			// 恢复不使用 mipmap 的设置
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mag);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void generateMipmap()
	{
		if (!useMipmap)return;
		glBindTexture(GL_TEXTURE_2D, gltexture_id);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void setFilterMode(FilterMode _mode)
	{
		if (_mode == magFilter)return;
		magFilter = _mode;

		applyMagfilterSettings();
	}
	void applyMagfilterSettings()
	{
		GLenum filter = getGlFilterMode(magFilter);
		glBindTexture(GL_TEXTURE_2D, gltexture_id);
		if (!useMipmap)glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	

	void setWrapMode(WrapMode _mode)
	{
		if (_mode == wrapMode)return;
		wrapMode = _mode;
		applyWrapSettings();
	}
	void applyWrapSettings()
	{
		GLenum wrap = getGlWrapMode(wrapMode);

		glBindTexture(GL_TEXTURE_2D, gltexture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void loadJson(const nlohmann::json& data);
	nlohmann::json toJson();

private:
	unsigned int gltexture_id;
	std::string filePath;


	int height;
	int width;
	int nrChannels;
	int format;

	bool useMipmap = false;
	WrapMode wrapMode = WrapMode::ClampToEdge;
	FilterMode magFilter = FilterMode::Nearest;
	FilterMode minFilter = FilterMode::Nearest;


	TextureResource( const std::string& filePath);
	TextureResource(size_t gl_id);
	TextureResource();


	static GLenum loadTextureFormat(int& nrChannels);
	static GLenum getGlWrapMode(WrapMode mode);
	static GLenum getGlFilterMode(FilterMode mode);
	static GLenum toGLTextureUnit(TextureChannel channel);
};