#include "TextureManager.h"   

#include <iostream>           
#include "TextureResource.h"


TextureManager::TextureManager() {
    return;
}

TextureManager::~TextureManager() {
    return;
}

TextureResource* TextureManager::getTextureResourceByPath(const std::string& _filePath)
{
	const auto& it = textureResourcesMap.find(_filePath);
	if (it!= textureResourcesMap.end())
	{
		return it->second.get();
	}

	std::unique_ptr<TextureResource>tr = std::unique_ptr<TextureResource>(new TextureResource(_filePath));
	TextureResource* rawPtr = tr.get();
	textureResourcesMap[_filePath] = std::move(tr);
	return rawPtr;
}

TextureResource* TextureManager::getTextureResourceByJson(const nlohmann::json& data)
{
	std::string filePath = data["filePath"];

	const auto& it = textureResourcesMap.find(filePath);
	if (it != textureResourcesMap.end())
	{
		return it->second.get();
	}

	std::unique_ptr<TextureResource>tr = std::unique_ptr<TextureResource>(new TextureResource());
	TextureResource* rawPtr = tr.get();
	tr->loadJson(data);
	textureResourcesMap[filePath] = std::move(tr);
	return rawPtr;
}


std::unique_ptr<TextureResource> TextureManager::getTextureResourceByGlid(size_t glTexture_id)
{
	std::unique_ptr<TextureResource>tr = std::unique_ptr<TextureResource>(new TextureResource(glTexture_id));
	
	return tr;
}




TextureManager& TextureManager::getInstance()
{
	static TextureManager instance;  
	return instance;
}




