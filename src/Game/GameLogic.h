#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include <memory>

class EntityManager;
class Level;
namespace GameLogic
{
	std::vector<sf::Vector2f> explosionSpawnPositions(const sf::Vector2f& entityPosition, 
		const EntityManager& entityManager, const std::unique_ptr<Level>& currentLevel);
	
	const sf::Vector2f& getPlayerPosition(const EntityManager& entityManager);
	const sf::Vector2f& getEntityPosition(const EntityManager& entityManager, const std::string& entityName);
}