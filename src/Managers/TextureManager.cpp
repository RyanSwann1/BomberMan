#include "TextureManager.h"
#include <Utilities\Utilities.h>
#include <Locators\TextureManagerLocator.h>

TextureManager::TextureManager()
	: ResourceManager(Utilities::getFileDirectories("res/Textures.txt"))
{
	TextureManagerLocator::provide(*this);
}

const sf::Texture & TextureManager::loadFromFile(const std::string & fileDirectory) const
{
	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile(fileDirectory);
	assert(texture);
	return *texture;
}
