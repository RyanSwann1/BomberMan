#include "GameLogic.h"
#include <Level\Level.h>
#include <Managers\EntityManager.h>
#include <Entities\CollisionHandler.h>

std::vector<sf::Vector2f> GameLogic::explosionSpawnPositions(const sf::Vector2f & entityPosition, 
	const EntityManager& entityManager, const std::unique_ptr<Level>& currentLevel)
{
	std::vector<sf::Vector2f> explosionSpawnPositions;
	const int tileSize = currentLevel->getTileSize();

	//Get X axis spawn positions
	const int entityXPosition(std::floor(entityPosition.x / tileSize));
	const int entityYPosition(std::floor(entityPosition.y / tileSize));
	for (int x = entityXPosition; x <= entityXPosition + 2; ++x)
	{
		if (CollisionHandler::isEntityAtPosition("Crate", sf::Vector2f(x * tileSize, entityYPosition * tileSize), entityManager))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			break;
		}

		if (!CollisionHandler::isTileAtPosition(sf::Vector2i(x, entityYPosition)))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			continue;
		}

		break;
	}

	for (int x = entityXPosition; x >= entityXPosition - 2; --x)
	{
		if (CollisionHandler::isEntityAtPosition("Crate", sf::Vector2f(x * tileSize, entityYPosition * tileSize), entityManager))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			break;
		}

		if (!CollisionHandler::isTileAtPosition(sf::Vector2i(x, entityYPosition)))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			continue;
		}

		break;
	}

	//Get Y Axis spawn positions
	for (int y = entityYPosition; y <= entityYPosition + 2; ++y)
	{
		if (CollisionHandler::isEntityAtPosition("Crate", sf::Vector2f(entityXPosition * tileSize, y * tileSize), entityManager))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			break;
		}

		if (!CollisionHandler::isTileAtPosition(sf::Vector2i(entityXPosition, y)))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			continue;
		}

		break;
	}

	for (int y = entityYPosition; y >= entityYPosition - 2; --y)
	{
		if (CollisionHandler::isEntityAtPosition("Crate", sf::Vector2f(entityXPosition * tileSize, y * tileSize), entityManager))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			break;
		}

		if (!CollisionHandler::isTileAtPosition(sf::Vector2i(entityXPosition, y)))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			continue;
		}

		break;
	}

	return explosionSpawnPositions;
}

const sf::Vector2f & GameLogic::getPlayerPosition(const EntityManager & entityManager)
{
	const auto& entities = entityManager.getEntities();
	auto iter = std::find_if(entities.cbegin(), entities.cend(), [](const auto& entity) {return entity->getName() == "Player"; });
	assert(iter != entities.cend());
	return iter->get()->getPosition();
}

const sf::Vector2f & GameLogic::getEntityPosition(const EntityManager & entityManager, const std::string & entityName)
{
	const auto& entities = entityManager.getEntities();
	auto iter = std::find_if(entities.cbegin(), entities.cend(), [&entityName](const auto& entity) {return entity->getName() == entityName; });
	assert(iter != entities.cend());
	return iter->get()->getPosition();
}