#include "CollisionHandler.h"
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <math.h>

void CollisionHandler::clampMovement(const sf::FloatRect& intersection, sf::Vector2f& movement)
{
	if (movement.x < 0.f)
	{
		movement.x += intersection.width;
	}
	else if (movement.x > 0.f)
	{
		movement.x -= intersection.width;
	}
	if (movement.y < 0.f)
	{
		movement.y += intersection.height;
	}
	else if (movement.y > 0.f)
	{
		movement.y -= intersection.height;
	}
}

bool CollisionHandler::isCollidableTileAtPosition(const sf::Vector2f & position, int tileSize)
{
	for (const auto& entity : EntityManagerLocator::getEntityManager().getEntities())
	{
		if (entity->isCollidable() && entity->getPosition() == sf::Vector2f(position.x * tileSize, position.y * tileSize))
		{
			return true;
		}
	}
	
	return false;
}

bool CollisionHandler::isEntityAtPosition(const sf::Vector2f & position, const EntityManager & entityManager, EntityTag entityTag, int tileSize)
{
	for (const auto& entity : entityManager.getEntities())
	{
		if (entity->getTag() != entityTag)
		{
			continue;
		}

		const sf::FloatRect rect(sf::Vector2f(position.x * tileSize, position.y * tileSize), sf::Vector2f(tileSize, tileSize));
		if (entity->getAABB().intersects(rect))
		{
			return true;
		}
	}

	return false;
}

void CollisionHandler::checkForEntityCollisions(const sf::Vector2f & entityPosition, const EntityManager & entityManager, sf::Vector2f & movement, Entity & entity)
{
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	for (const auto& i : entityManager.getEntities())
	{
		//Dont check collision on self
		if (i->getID() == entity.getID())
		{
			continue;
		}

		const sf::FloatRect movementAABB(entityPosition + movement, sf::Vector2f(tileSize, tileSize));
		sf::FloatRect intersection;
		if (movementAABB.intersects(i->getAABB(), intersection))
		{
			entity.handleEntityCollision(i, intersection);

			if (i->isCollidable())
			{
				CollisionHandler::clampMovement(intersection, movement);
			}
		}
	}
}