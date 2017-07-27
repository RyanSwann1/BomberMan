#include "Explosion.h"
#include <Managers\EntityManager.h>

Explosion::Explosion(const std::string& name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID)
	: Entity(name, tag, spawnPosition, entityManager, ID)
{}

void Explosion::update(float deltaTime)
{
	Entity::update(deltaTime);

	if (m_animationPlayer.getCurrentAnimation(AnimationName::Default).isFinished())
	{
		m_entityManager.removeEntity(Entity::getID());
	}
}
