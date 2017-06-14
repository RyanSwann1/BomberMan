#include "Bomb.h"
#include <Managers\EntityManager.h>

Bomb::Bomb(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Entity(name, position, entityManager, entityID)
{
	m_animationPlayer.play("Bomb");
}

void Bomb::handleAnimation()
{
	if (m_animationPlayer.getCurrentAnimation().getName() == "Bomb" && m_animationPlayer.getCurrentAnimation().isFinished())
	{
		m_animationPlayer.play("Explode");
	}

	else if (m_animationPlayer.getCurrentAnimation().getName() == "Explode" && m_animationPlayer.getCurrentAnimation().isFinished())
	{
		m_entityManager.removeEntity(Entity::getID());
	}
}
