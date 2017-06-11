#pragma once

#include <SFML\Graphics.hpp>
#include <Tile\AnimationPlayer.h>
#include "Direction.h"
#include <Locators\LevelManagerLocator.h>

class EntityManager;
class Entity : private LevelManagerLocator
{
public:
	Entity(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);

	void draw(sf::RenderWindow& window);
	void update(float deltaTime);
	
private:
	EntityManager& m_entityManager;
	AnimationPlayer m_animationPlayer;
	const int m_ID;
	sf::Vector2f m_position;
	sf::Vector2f m_speed;
	Direction m_currentMoveDirection;

	void handleDirection(const sf::Vector2f& movement);
};