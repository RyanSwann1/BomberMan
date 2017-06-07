#pragma once

#include <SFML\Graphics.hpp>
class EntityManager;
class Entity
{
public:
	Entity(const sf::Vector2f& position, EntityManager& entityManager, int entityID);
	~Entity();

	void draw(sf::RenderWindow& window);
	void update(float deltaTime);
	
private:
	EntityManager& m_entityManager;
	const int m_ID;
	sf::Sprite m_sprite;
	sf::Vector2f m_position;

};