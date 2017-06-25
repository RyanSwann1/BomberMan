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


}