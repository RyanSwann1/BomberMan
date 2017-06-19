#include "Crate.h"
#include "CollisionHandler.h"
#include <Managers\EntityManager.h>

Crate::Crate(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Entity(name, position, entityManager, entityID)
{
}

void Crate::update(float deltaTime)
{
	CollisionHandler::handleCollisions(m_position, m_entityManager, sf::Vector2f(), *this);
}

void Crate::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect & intersection)
{
	if (entity->getName() == "Explosion")
	{
		
		m_entityManager.removeEntity(Entity::getID());
	}
}
