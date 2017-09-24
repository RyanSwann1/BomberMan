#include "Explosion.h"
#include <Managers\EntityManager.h>
#include <Entities\CollisionHandler.h>
#include <Game\GameLogic.h>

Explosion::Explosion(const std::string& name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID, bool collidable)
	: Entity(name, tag, spawnPosition, entityManager, ID, collidable)
{}

void Explosion::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect & intersection)
{
	switch (entity->getTag())
	{
	case EntityTag::Crate :
	{
		m_entityManager.removeEntity(entity->getID());
		GameLogic::spawnPowerUp(m_position, m_entityManager);
		break;
	}
	}
}

void Explosion::update(float deltaTime)
{
	Entity::update(deltaTime);

	CollisionHandler::checkForEntityCollisions(m_position, m_entityManager, sf::Vector2f(), *this);

	if (m_animationPlayer.getCurrentAnimation(AnimationName::Default).isFinished())
	{
		m_entityManager.removeEntity(Entity::getID());
	}
}
