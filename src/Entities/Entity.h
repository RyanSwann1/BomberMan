#pragma once

#include <SFML\Graphics.hpp>
#include <Tile\AnimationPlayer.h>

class EntityManager;
class Entity
{
public:
	Entity(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);

	int getID() const;

	void draw(sf::RenderWindow& window);
	virtual void update(float deltaTime);
	
protected:
	sf::Vector2f m_speed;
	sf::Vector2f m_velocity;
	sf::Vector2f m_position;
	AnimationPlayer m_animationPlayer;
	EntityManager& m_entityManager;
	
	Direction getCurrentMoveDirection() const;
	virtual void handleAnimation() {}

private:
	const int m_ID;
	Direction m_currentMoveDirection;

	void handleDirection(const sf::Vector2f& movement);
};