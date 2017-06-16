#pragma once

#include <SFML\Graphics.hpp>
#include <Tile\AnimationPlayer.h>

class EntityManager;
class Entity
{
public:
	Entity(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);
	virtual ~Entity() {}

	int getID() const;
	void draw(sf::RenderWindow& window);
	virtual void update(float deltaTime);
	
protected:
	sf::Vector2f m_position;
	AnimationPlayer m_animationPlayer;
	EntityManager& m_entityManager;

private:
	const int m_ID;
};