#pragma once

#include <Managers\ResourceManager.h>
#include <Locators\TextureManagerLocator.h>
#include <Utilities\Utilities.h>
#include <SFML\Graphics.hpp>

class TextureManager : public ResourceManager<sf::Texture>
{
public:
	TextureManager()
		: ResourceManager(Utilities::getFileDirectories("res/Textures.txt"))
	{
		TextureManagerLocator::provide(*this);
	}

private:
	const sf::Texture& loadFromFile(const std::string& fileDirectory) const override
	{
		sf::Texture* texture = new sf::Texture();
		texture->loadFromFile(fileDirectory);
		assert(texture);
		return *texture;
	}
};