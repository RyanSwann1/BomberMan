#include <Entities\PowerUp.h>
#include <Managers\EntityManager.h>
#include <Entities\CollisionHandler.h>

PowerUp::PowerUp(const std::string & name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID, bool collidable)
	: Entity(name, tag, spawnPosition, entityManager, ID, collidable),
	m_spawnImmunityTimer(1.0f, true)
{
}

void PowerUp::update(float deltaTime)
{
	CollisionHandler::checkForEntityCollisions(m_position, m_entityManager, sf::Vector2f(), *this);
	m_spawnImmunityTimer.update(deltaTime);
	if (m_spawnImmunityTimer.isExpired())
	{
		m_spawnImmunityTimer.reset();
		m_spawnImmunityTimer.deactivate();
	}
}

void PowerUp::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect & intersection)
{
	switch (entity->getTag())
	{
	case EntityTag::Explosion:
	{
		if (m_spawnImmunityTimer.isActive())
		{
			return;
		}
		m_entityManager.removeEntity(Entity::getID());
		break;
	}
	case EntityTag::Enemy:
	{
		m_entityManager.removeEntity(Entity::getID());
		break;
	}
	case EntityTag::Player:
	{
		m_entityManager.removeEntity(Entity::getID());
		break;
	}
	}
}
