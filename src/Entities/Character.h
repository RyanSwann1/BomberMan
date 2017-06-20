#pragma once

#include "Entity.h"
#include <Game\Timer.h>

class Character : public Entity
{
public:
	Character(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);
	virtual ~Character() {}

	void update(float deltaTime) override;
	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;

protected:
	sf::Vector2f m_speed;
	sf::Vector2f m_velocity;
	Direction m_currentMoveDirection;
	int m_lives;
	Timer m_bombPlacementTimer;
	
	void placeBomb();

private:
	void handleDirection();	
	void handleAnimation();
};