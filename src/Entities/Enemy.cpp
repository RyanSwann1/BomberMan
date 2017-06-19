#include "Enemy.h"
#include <Managers\EntityManager.h>
#include <Locators\AudioClipPlayerLocator.h>
#include <Audio\AudioClipPlayer.h>

Enemy::Enemy(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Character(name, position, entityManager, entityID)
{
	
}

void Enemy::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection)
{
	if (entity->getName() == "Explosion")
	{
		m_entityManager.removeEntity(Entity::getID());
		AudioClipPlayerLocator::getAudioClipPlayer().playSound(AudioClipName::EnemyDeath);
	}
}
