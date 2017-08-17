#include <Entities\BombMediumPower.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Entities\CollisionHandler.h>

BombMediumPower::BombMediumPower(const std::string & name, EntityTag tag, const sf::Vector2f & position, EntityManager & entityManager, int entityID, bool collidable)
	: Bomb(name, tag, position, entityManager, entityID, collidable, BombPower::Medium, 2)
{
}

std::vector<sf::Vector2f> BombMediumPower::getExplosionSpawnPositions() const
{
	std::vector<sf::Vector2f> explosionSpawnPositions;
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();

	//Get X axis spawn positions
	const int entityXPosition(std::floor(m_position.x / tileSize));
	const int entityYPosition(std::floor(m_position.y / tileSize));
	for (int x = entityXPosition; x <= entityXPosition + m_explosionRadius; ++x)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(x, entityYPosition), m_entityManager, EntityTag::Crate, tileSize))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			break;
		}

		if (!CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(x, entityYPosition), tileSize))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			continue;
		}

		break;
	}

	for (int x = entityXPosition; x >= entityXPosition - m_explosionRadius; --x)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(x, entityYPosition), m_entityManager, EntityTag::Crate, tileSize))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			break;
		}

		if (!CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(x, entityYPosition), tileSize))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			continue;
		}

		break;
	}

	//Get Y Axis spawn positions
	for (int y = entityYPosition; y <= entityYPosition + m_explosionRadius; ++y)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(entityXPosition, y), m_entityManager, EntityTag::Crate, tileSize))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			break;
		}

		if (!CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(entityXPosition, y), tileSize))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			continue;
		}

		break;
	}

	for (int y = entityYPosition; y >= entityYPosition - m_explosionRadius; --y)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(entityXPosition, y), m_entityManager, EntityTag::Crate, tileSize))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			break;
		}

		if (!CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(entityXPosition, y), tileSize))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			continue;
		}

		break;
	}

	return explosionSpawnPositions;
}
