#pragma once

#include <vector>
#include <SFML\Graphics.hpp>

class CollisionLayer
{
public:
	CollisionLayer(std::vector<sf::Vector2i>&& collidablePositions);



private:
	const std::vector<sf::Vector2i> m_collidablePositions;
};