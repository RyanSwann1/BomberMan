#include "GameLogic.h"
#include <Level\Level.h>

std::vector<sf::Vector2f> GameLogic::explosionSpawnPositions(const sf::Vector2f & entityPosition, const Level & currentLevel)
{
	std::vector<sf::Vector2f> explosionSpawnPositions;
	const int tileSize = currentLevel.getTileSize();

	//Get X axis spawn positions
	const int entityXPosition(std::floor(entityPosition.x / tileSize));
	const int entityYPosition(std::floor(entityPosition.y / tileSize));
	for (int x = entityXPosition; x <= entityXPosition + 2; ++x)
	{
		if (!currentLevel.collisionTileAtPosition(sf::Vector2i(x, entityYPosition)))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			continue;
		}

		break;
	}

	for (int x = entityXPosition; x >= entityXPosition - 2; --x)
	{
		if (!currentLevel.collisionTileAtPosition(sf::Vector2i(x, entityYPosition)))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			continue;
		}

		break;
	}

	//Get Y Axis spawn positions
	for (int y = entityYPosition; y <= entityYPosition + 2; ++y)
	{
		if (!currentLevel.collisionTileAtPosition(sf::Vector2i(entityXPosition, y)))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			continue;
		}

		break;
	}

	for (int y = entityYPosition; y >= entityYPosition - 2; --y)
	{
		if (!currentLevel.collisionTileAtPosition(sf::Vector2i(entityXPosition, y)))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			continue;
		}

		break;
	}

	return explosionSpawnPositions;
}
