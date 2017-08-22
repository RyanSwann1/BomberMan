#include "GameLogic.h"
#include <Level\Level.h>
#include <Managers\EntityManager.h>
#include <Entities\CollisionHandler.h>
#include <Entities\EntityTag.h>
#include <Game\RandomNumberGenerator.h>

void GameLogic::spawnPowerUp(const sf::Vector2f& position, EntityManager& entityManager)
{
	if (RandomNumberGenerator::getRandomNumber(0, 10) < 7)
	{
		return;
	}

	const int powerUpSpawnOption = RandomNumberGenerator::getRandomNumber(1, 3);
 	switch (powerUpSpawnOption)
	{
	case 1:
	{
		entityManager.addEntity("PowerUpSpeedBoost", position);
		break;
	}
	case 2:
	{
		entityManager.addEntity("PowerUpExtraBomb", position);
		break;
	}
	case 3:
	{
		entityManager.addEntity("PowerUpIncreaseBombPower", position);
		break;
	}
	}
}

const sf::Vector2f & GameLogic::getEntityPosition(const EntityManager & entityManager, EntityTag entityTag)
{
	const auto& entities = entityManager.getEntities();
	auto iter = std::find_if(entities.cbegin(), entities.cend(), [entityTag](const auto& entity) {return entity->getTag() == entityTag; });
	assert(iter != entities.cend());
	return iter->get()->getPosition();
}

const sf::Vector2f & GameLogic::getPlayerPosition(const EntityManager & entityManager)
{
	const auto& entities = entityManager.getEntities();
	auto iter = std::find_if(entities.cbegin(), entities.cend(), [](const auto& entity) { return entity->getTag() == EntityTag::Player; });
	assert(iter != entities.cend());
	return iter->get()->getPosition();
}

bool GameLogic::isEntityAtPosition(const EntityManager & entityManager, const sf::Vector2i & position, EntityTag entityTag)
{
	for (const auto& entity : entityManager.getEntities())
	{
		if (entity->getTag() != entityTag)
		{
			continue;
		}

		const auto& entityPosition = sf::Vector2i(std::round(entity->getPosition().x), std::round(entity->getPosition().y));
		if (entityPosition == position)
		{
			return true;
		}
	}

	return false;
}

const std::unique_ptr<Entity>* GameLogic::getEntityAtPosition(const EntityManager & entityManager, const sf::Vector2f & position, EntityTag entityTag, int tileSize)
{
	for (const auto& entity : entityManager.getEntities())
	{
		if (entity->getTag() != entityTag)
		{
			continue;
		}

		const auto& entityPosition = sf::Vector2i(std::round(entity->getPosition().x / tileSize), std::round(entity->getPosition().y / tileSize));
		if (entityPosition == sf::Vector2i(std::round(position.x / tileSize), std::round(position.y / tileSize)))
		{
			return &entity;
		}
	}

	return nullptr;
}