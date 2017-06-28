#pragma once

#include <SFML\Graphics.hpp>
#include <Tile\AnimationPlayer.h>

class EntityManager;
class Entity
{
public:
	Entity(const std::string& entityName, const sf::Vector2f& entityPosition, EntityManager& entityManager, int entityID);
	virtual ~Entity() {}

	const std::string& getName() const;
	const sf::FloatRect& getAABB() const;
	const sf::Vector2f& getPosition() const;
	int getID() const;
	
	virtual void draw(sf::RenderWindow& window);
	virtual void update(float deltaTime);
	virtual void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) {}
	
protected:
	sf::Vector2f m_position;
	sf::FloatRect m_AABB;
	AnimationPlayer m_animationPlayer;
	EntityManager& m_entityManager;

private:
	const int m_ID;
	const std::string m_name;
	sf::RectangleShape m_debugRect;

	void updateAABB();
};