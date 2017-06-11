#include "CollisionHandler.h"
#include <Level\Level.h>

void CollisionHandler::ClampMovement(sf::Vector2f& movement, const sf::Vector2f& entityPosition, const Level & level)
{
	const int tileSize = level.getTileSize();
	const sf::FloatRect entityAABB(entityPosition + movement, sf::Vector2f(tileSize / 2.0f, tileSize / 2.0f));
	for (const auto& collidableTile : level.getCollisionLayer())
	{
		sf::FloatRect intersection;
		const sf::FloatRect tileRect(sf::Vector2f(collidableTile.x, collidableTile.y), sf::Vector2f(tileSize, tileSize));
		if (!entityAABB.intersects(tileRect, intersection))
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
