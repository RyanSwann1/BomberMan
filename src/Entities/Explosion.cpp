#include "Explosion.h"
#include <Managers\EntityManager.h>

Explosion::Explosion(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Entity(name, position, entityManager, entityID)
{
	m_animationPlayer.play("Default");
}

void Explosion::update(float deltaTime)
{
	Entity::update(deltaTime);

	if (m_animationPlayer.getCurrentAnimation("Default").isFinished())
	{
		m_entityManager.removeEntity(Entity::getID());
	}
}
