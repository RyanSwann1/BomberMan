#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
class Level;
namespace GameLogic
{
	std::vector<sf::Vector2f> explosionSpawnPositions(const sf::Vector2f& entityPosition, const Level& currentLevel);
}