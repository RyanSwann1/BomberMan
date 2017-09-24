#pragma once

#include <SFML\Graphics.hpp>
#include <memory>

enum class EntityTag;
class Entity;
class EntityManager;
namespace GameLogic
{	
	void removeEntityAtPosition(const sf::Vector2f& position, EntityManager& entityManager);
	void spawnPowerUp(const sf::Vector2f& position, EntityManager& entityManager);
	
	const sf::Vector2f& getPlayerPosition(const EntityManager& entityManager);
	const std::unique_ptr<Entity>* getEntityAtPosition(const EntityManager& entityManager, const sf::Vector2f& position, EntityTag entityTag, int tileSize);
	const std::unique_ptr<Entity>* getEntityAtPosition(const EntityManager& entityManager, const sf::Vector2f& position, int tileSize);
	bool isPlayerAlive();
}