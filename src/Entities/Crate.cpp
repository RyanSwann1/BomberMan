#include "Crate.h"
#include "CollisionHandler.h"
#include <Managers\EntityManager.h>
#include <Managers\LevelManager.h>
#include <Locators\LevelManagerLocator.h>

Crate::Crate(const std::string& name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID)
	: Entity(name, tag, spawnPosition, entityManager, ID)
{
}

void Crate::update(float deltaTime)
{
	CollisionHandler::handleCollisions(m_position, m_entityManager, sf::Vector2f(), *this);
}

void Crate::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect & intersection)
{
	if (entity->getTag() == EntityTag::Explosion)
	{
		m_entityManager.removeEntity(Entity::getID());
	}
}
