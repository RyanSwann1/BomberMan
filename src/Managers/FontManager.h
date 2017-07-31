#pragma once

#include <Managers\ResourceManager.h>
#include <SFML\Graphics.hpp>

class FontManager : public ResourceManager<sf::Font>
{
public:
	FontManager();

	const sf::Font& loadFromFile(const std::string& fileName) const;
};