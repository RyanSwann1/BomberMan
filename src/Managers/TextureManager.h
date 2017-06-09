#pragma once

#include <Managers\ResourceManager.h>
#include <SFML\Graphics.hpp>

class TextureManager : public ResourceManager<sf::Texture>
{

public:
	TextureManager();

private:
	const sf::Texture& loadFromFile(const std::string& fileDirectory) const override;
};