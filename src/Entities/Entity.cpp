#include "Entity.h"
#include "CollisionHandler.h"
#include <Tile\TileSheet.h>
#include <Managers\LevelManager.h>
#include <math.h>

Entity::Entity(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID)
	: m_entityManager(entityManager),
	m_animationPlayer(name),
	m_ID(entityID),
	m_position(position)
{
	m_animationPlayer.play("Default");
}

void Entity::draw(sf::RenderWindow & window)
{
	m_animationPlayer.draw(m_position, window);
}

void Entity::update(float deltaTime)
{
	m_animationPlayer.update(deltaTime);
}

int Entity::getID() const
{
	return m_ID;
}