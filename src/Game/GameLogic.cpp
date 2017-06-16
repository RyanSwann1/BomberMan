#include "GameLogic.h"
#include <Level\Level.h>


std::vector<sf::Vector2f> GameLogic::explosionSpawnPositions(const sf::Vector2f & entityPosition, const Level & currentLevel)
{
	std::vector<sf::Vector2f> explosionSpawnPositions;
	const int tileSize = currentLevel.getTileSize();

	//Get X axis spawn positions
	const float entityXPosition(std::floor(entityPosition.x / tileSize));
	const float entityYPosition(std::floor(entityPosition.y / tileSize));
	for (int x = entityXPosition - 1; x <= entityXPosition + 1; ++x)
	{
		if (!currentLevel.collisionTileAtPosition(sf::Vector2f(x, entityYPosition)))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
		}
	}

	//Get Y Axis spawn positions
	for (int y = entityYPosition - 1; y <= entityYPosition + 1; ++y)
	{
		if (!currentLevel.collisionTileAtPosition(sf::Vector2f(entityXPosition, y)))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
		}
	}

	return explosionSpawnPositions;
}
