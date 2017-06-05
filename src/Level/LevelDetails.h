#pragma once

#include <SFML\Graphics.hpp>

class LevelDetails
{
public:
	LevelDetails(const sf::Vector2i& levelSize, int tileSize)
		: m_size(levelSize),
		m_tileSize(tileSize)
	{}

	const sf::Vector2i m_size;
	const int m_tileSize;
};