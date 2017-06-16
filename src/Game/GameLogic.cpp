#include "GameLogic.h"
#include <Level\Level.h>

std::vector<sf::Vector2f> GameLogic::explosionSpawnPositions(const sf::Vector2f & entityPosition, const Level & currentLevel)
{
	std::vector<sf::Vector2f> explosionSpawnPositions;
	const int tileSize = currentLevel.getTileSize();

	const float entityXPosition(std::floor(entityPosition.x / tileSize));
	const float entityYPosition(std::floor(entityPosition.y / tileSize));
	for (int x = entityXPosition - 1; x <= entityXPosition + 1; ++x)
	{
		if (!currentLevel.collisionTileAtPosition(sf::Vector2f(x, entityYPosition)))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
		}
	}

	return explosionSpawnPositions;
}
