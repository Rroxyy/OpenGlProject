#include "TextureManager.h"   

#include <iostream>           
#include "TextureResource.h"


TextureManager::TextureManager() {
    nowIndex = 0;
    return;
}

TextureManager::~TextureManager() {
    return;
}

TextureResource* TextureManager::createTextureResourceByPath(const std::string& _filePath)
{
	const auto& it = filePathMap.find(_filePath);
	if (it!=filePathMap.end())
	{
		std::cout << "[TextureManager] Texture already loaded: " << _filePath
			<< " (ID = " << it->second << ")" << std::endl;

		return textureResourcesMap[it->second].get();
	}

	std::unique_ptr<TextureResource>tr = std::unique_ptr<TextureResource>(new TextureResource(nowIndex, _filePath));
	TextureResource* rawPtr = tr.get();
	textureResourcesMap[nowIndex] = std::move(tr);
	filePathMap[_filePath] = nowIndex;
	nowIndex++;
	return rawPtr;
}

std::unique_ptr<TextureResource> TextureManager::createTextureResourceByGlid(size_t glTexture_id)
{
	std::unique_ptr<TextureResource>tr = std::unique_ptr<TextureResource>(new TextureResource(glTexture_id));
	
	return tr;
}


TextureResource* TextureManager::getTextureByPath(const std::string& _filePath) const
{
	auto it = filePathMap.find(_filePath);
	if (it != filePathMap.end())
		return textureResourcesMap.at(it->second).get();
	return nullptr;
}


TextureManager& TextureManager::getInstance()
{
	static TextureManager instance;  
	return instance;
}

//TextureChannel TextureManager::getTexChannel(TextureResource* tr)
//{
//	if (!tr) {
//		std::cerr << "tr is null!" << std::endl;
//		std::abort();
//	}
//	if (!tr->filePath) {
//		std::cerr << "filePath is null!" << std::endl;
//		std::abort();
//	}
//
//	if (nowIndex>=32)
//	{
//		std::cerr << "Failed to get texture channel: " << tr->filePath <<std::endl
//		<< "Because no channel" << std::endl;
//		std::abort();
//	}
//	if (m.find(tr)==m.end())
//	{
//		m[tr] = static_cast<TextureChannel>(static_cast<int>(TextureChannel::Texture0) + nowIndex);
//		nowIndex++;
//	}
//	//std::cout << tr->filePath << "    " << "textureChannel" << static_cast<int>(m[tr]) << std::endl;
//	return m[tr];
//}


