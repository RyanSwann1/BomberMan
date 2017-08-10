#pragma once

#include "Entity.h"
#include <Game\Timer.h>

class Character : public Entity
{
public:
	Character(const std::string& name, EntityTag tag, const sf::Vector2f& spawnPosition, EntityManager& entityManager, int ID, bool collidable);
	virtual ~Character() {}

	void update(float deltaTime) override;
	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;

protected:
	const sf::Vector2f m_maxSpeed;
	sf::Vector2f m_speed;
	sf::Vector2f m_velocity;
	sf::Vector2f m_oldPosition;
	Direction m_currentMoveDirection;
	int m_lives;
	
	void increaseSpeed(float x, float y);

private:
	void handleDirection();	
	void handleAnimation();
	void resetVelocity();
};