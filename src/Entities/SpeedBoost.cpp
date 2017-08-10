#include "SpeedBoost.h"
#include <Managers\EntityManager.h>
#include <Entities\CollisionHandler.h>

SpeedBoost::SpeedBoost(const std::string & name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID, bool collidable)
	: Entity(name, tag, spawnPosition, entityManager, ID, collidable)
{
}

void SpeedBoost::update(float deltaTime)
{
	Entity::update(deltaTime);
}

void SpeedBoost::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect & intersection)
{
	switch (entity->getTag())
	{
	case EntityTag::Explosion :
	{
		m_entityManager.removeEntity(Entity::getID());
		break;
	}
	case EntityTag::Enemy :
	{
		m_entityManager.removeEntity(Entity::getID());
		break;
	}
	case EntityTag::Player :
	{
		m_entityManager.removeEntity(Entity::getID());
		break;
	}
	}
}