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
TextureManager& TextureManager::getInstance()
{
	static TextureManager instance;  // 局部静态变量，线程安全（C++11 起）
	return instance;
}

TextureChannel TextureManager::getTexChannel(TextureResource* tr)
{
	if (!tr) {
		std::cerr << "tr is null!" << std::endl;
		std::abort();
	}
	if (!tr->filePath) {
		std::cerr << "filePath is null!" << std::endl;
		std::abort();
	}

	if (nowIndex>=32)
	{
		std::cerr << "Failed to get texture channel: " << tr->filePath <<std::endl
		<< "Because no channel" << std::endl;
		std::abort();
	}
	if (m.find(tr)==m.end())
	{
		m[tr] = static_cast<TextureChannel>(static_cast<int>(TextureChannel::Texture0) + nowIndex);
		nowIndex++;
	}
	//std::cout << tr->filePath << "    " << "textureChannel" << static_cast<int>(m[tr]) << std::endl;
	return m[tr];
}

