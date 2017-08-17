#include "Crate.h"
#include "CollisionHandler.h"
#include <Managers\EntityManager.h>
#include <Managers\LevelManager.h>
#include <Locators\LevelManagerLocator.h>
#include <Game\GameLogic.h>

Crate::Crate(const std::string& name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID, bool collidable)
	: Entity(name, tag, spawnPosition, entityManager, ID, collidable)
{
}

void Crate::update(float deltaTime)
{
	CollisionHandler::checkForEntityCollisions(m_position, m_entityManager, sf::Vector2f(), *this);
}

void Crate::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect & intersection)
{
	if (entity->getTag() == EntityTag::Explosion)
	{
		m_entityManager.removeEntity(Entity::getID());
		GameLogic::spawnPowerUp(m_position, m_entityManager);
	}
}
