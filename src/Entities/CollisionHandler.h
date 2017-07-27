#pragma once

#include <SFML\Graphics.hpp>

enum class EntityTag;
class EntityManager;
class Entity;
namespace CollisionHandler
{
	void handleCollisions(const sf::Vector2f& entityPosition, const EntityManager& entityManager, sf::Vector2f& movement, Entity& entity);
	void clampMovement(const sf::FloatRect& intersection, sf::Vector2f& movement);
	bool isCollidableTileAtPosition(const sf::Vector2i& position);
	bool isEntityAtPosition(EntityTag entityTag, const sf::Vector2f& position, const EntityManager& entityManager);
}