#include "Entity.h"

Entity::Entity(const sf::Vector2f & position, EntityManager& entityManager, int entityID)
	: m_entityManager(entityManager),
	m_ID(entityID),
	m_position(position),
	m_sprite()
{
}

Entity::~Entity()
{
}

void Entity::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

void Entity::update(float deltaTime)
{

}
