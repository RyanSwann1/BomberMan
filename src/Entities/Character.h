#pragma once

#include "Entity.h"

class Character : public Entity
{
public:
	Character(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);
	virtual ~Character() {}

	void update(float deltaTime) override;

protected:
	sf::Vector2f m_speed;
	sf::Vector2f m_velocity;
	Direction m_currentMoveDirection;

private:
	void handleDirection();	
	void handleAnimation();
};