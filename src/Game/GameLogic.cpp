#include "GameLogic.h"
#include <Level\Level.h>
#include <Managers\EntityManager.h>
#include <Entities\CollisionHandler.h>
#include <Entities\EntityTag.h>
#include <Game\RandomNumberGenerator.h>
#include <Locators\LevelManagerLocator.h>
#include <Locators\EntityManagerLocator.h>
#include <Managers\LevelManager.h>

void GameLogic::removeEntityAtPosition(const sf::Vector2f & position, EntityManager & entityManager)
{
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	const auto* entityAtPosition = getEntityAtPosition(entityManager, position, tileSize);
	if (entityAtPosition)
	{
		entityManager.removeEntity(entityAtPosition->get()->getID());
	}
}

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

const sf::Vector2f & GameLogic::getPlayerPosition(const EntityManager & entityManager)
{
	return entityManager.getEntity(EntityTag::Player)->getPosition();
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

const std::unique_ptr<Entity>* GameLogic::getEntityAtPosition(const EntityManager & entityManager, const sf::Vector2f & position, int tileSize)
{
	const auto entityPosition = sf::Vector2i((std::floor(position.x / tileSize) * tileSize), std::floor(position.y / tileSize) * tileSize);
	const auto& entities = entityManager.getEntities();
	for (const auto& entity : entities)
	{
		const auto position = sf::Vector2i(std::floor(entity->getPosition().x / tileSize) * tileSize, 
			std::floor(entity->getPosition().y / tileSize) * tileSize);

		if (entityPosition == position)
		{
			return &entity;
		}
	}
	
	return nullptr;
}

bool GameLogic::isPlayerAlive()
{
	const auto& entities = EntityManagerLocator::getEntityManager().getEntities();
	for (const auto& entity : entities)
	{
		if (!entity)
		{
			continue;
		}

		if (entity->getTag() == EntityTag::Player)
		{
			return true;
		}
	}

	return false;
}