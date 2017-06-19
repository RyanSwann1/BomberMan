#include "CollisionHandler.h"
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Managers\EntityManager.h>
#include <math.h>

void checkForEntityCollisions(const sf::Vector2f & entityPosition, const EntityManager & entityManager, const sf::Vector2f & movement, Entity & entity);
void clampMovement(const sf::Vector2f & entityPosition, sf::Vector2f & movement);

void CollisionHandler::handleCollisions(const sf::Vector2f & entityPosition, const EntityManager & entityManager, sf::Vector2f & movement, Entity & entity)
{
	checkForEntityCollisions(entityPosition, entityManager, movement, entity);
	clampMovement(entityPosition, movement);
}

void checkForEntityCollisions(const sf::Vector2f & entityPosition, const EntityManager & entityManager, const sf::Vector2f & movement, Entity & entity)
{
	const auto& level = LevelManagerLocator::getLevelManager().getCurrentLevel();
	const int tileSize = level.getTileSize();
	for (const auto& i : entityManager.getEntities())
	{
		if (i->getID() == entity.getID())
		{
			continue;
		}

		const sf::FloatRect movementAABB(entityPosition + movement, sf::Vector2f(tileSize, tileSize));
		sf::FloatRect intersection;
		if (movementAABB.intersects(i->getAABB(), intersection))
		{
			entity.handleEntityCollision(i, intersection);
			break;
		}
	}
}

void clampMovement(const sf::Vector2f & entityPosition, sf::Vector2f & movement)
{
	if (movement.x == 0 && movement.y == 0)
	{
		return;
	}

	const auto& level = LevelManagerLocator::getLevelManager().getCurrentLevel();
	const int tileSize = level.getTileSize();
	const sf::FloatRect movementAABB(entityPosition + movement, sf::Vector2f(tileSize, tileSize));
	for (const auto& collidableTile : level.getCollisionLayer())
	{
		sf::FloatRect intersection;
		const sf::FloatRect tileAABB(sf::Vector2f(collidableTile.x + 4, collidableTile.y + 4),
			sf::Vector2f(tileSize / 2.0f, tileSize / 2.0f));
		if (!movementAABB.intersects(tileAABB, intersection))
		{
			continue;
		}

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

		break;
	}
}
