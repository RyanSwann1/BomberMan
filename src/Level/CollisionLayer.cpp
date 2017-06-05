#include "CollisionLayer.h"
#include <utility>

CollisionLayer::CollisionLayer(std::vector<sf::Vector2i>&& collidablePositions)
	: m_collidablePositions(std::move(collidablePositions))
{
}
