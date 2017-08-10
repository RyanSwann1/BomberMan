#pragma once

#include <Entities\EntityTag.h>
#include <SFML\Graphics.hpp>
#include <Animation\AnimationPlayer.h>
#include <string>

class EntityManager;
class Entity
{
public:
	Entity(const std::string& name, EntityTag tag, const sf::Vector2f& spawnPosition, EntityManager& entityManager, int ID, bool collidable);
	virtual ~Entity() {}

	bool isCollidable() const;
	EntityTag getTag() const;
	const std::string& getName() const;
	const sf::FloatRect& getAABB() const;
	const sf::Vector2f& getPosition() const;
	int getID() const;
	
	void draw(sf::RenderWindow& window);
	virtual void update(float deltaTime);
	virtual void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) {}
	
protected:
	sf::Vector2f m_position;
	sf::FloatRect m_AABB;
	AnimationPlayer m_animationPlayer;
	EntityManager& m_entityManager;

	void setPosition(float x, float y);

private:
	const int m_ID;
	const EntityTag m_tag;
	const std::string m_name;
	const bool m_collidable;

	void updateAABB();
};