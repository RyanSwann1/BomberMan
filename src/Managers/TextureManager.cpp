#include "TextureManager.h"
#include <Utilities\Utilities.h>
#include <Locators\TextureManagerLocator.h>

TextureManager::TextureManager()
	: ResourceManager(Utilities::getFileDirectories(Utilities::getResourceDirectory() + "Textures.txt"))
{
	TextureManagerLocator::provide(*this);
}

const sf::Texture & TextureManager::loadFromFile(const std::string & fileDirectory) const
{
	sf::Texture* texture = new sf::Texture();
	const bool textureLoaded = texture->loadFromFile(fileDirectory);
	assert(textureLoaded);
	return *texture;
}
