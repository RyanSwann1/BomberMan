#pragma once

#include <SFML\Graphics.hpp>
#include <Tile\AnimationPlayer.h>
class EntityManager;
class Entity
{
public:
	Entity(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);
	~Entity();

	void draw(sf::RenderWindow& window);
	void update(float deltaTime);
	
private:
	EntityManager& m_entityManager;
	AnimationPlayer m_animationPlayer;
	const int m_ID;
	sf::Vector2f m_position;
};