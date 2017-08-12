#pragma once

#include <SFML\Graphics.hpp>
#include <memory>

enum class EntityTag;
class Entity;
class EntityManager;
namespace GameLogic
{	
	bool spawnPowerUp();
	const sf::Vector2f& getEntityPosition(const EntityManager& entityManager, EntityTag entityTag);
	const sf::Vector2f& getPlayerPosition(const EntityManager& entityManager);
	bool isEntityAtPosition(const EntityManager& entityManager, const sf::Vector2i& position, EntityTag entityTag);
	const std::unique_ptr<Entity>* getEntityAtPosition(const EntityManager& entityManager, const sf::Vector2f& position, EntityTag entityTag, int tileSize);
}