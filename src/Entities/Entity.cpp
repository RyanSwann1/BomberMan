#include "Entity.h"

Entity::Entity(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID)
	: m_entityManager(entityManager),
	m_animationPlayer(name),
	m_ID(entityID),
	m_position(position)
{
	m_animationPlayer.play("Idle");
}

Entity::~Entity()
{
}

void Entity::draw(sf::RenderWindow & window)
{
	m_animationPlayer.draw(m_position, window);
}

void Entity::update(float deltaTime)
{
	m_animationPlayer.update(deltaTime);
}
