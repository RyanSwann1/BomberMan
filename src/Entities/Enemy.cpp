#include "Enemy.h"
#include <Managers\EntityManager.h>
#include <Locators\AudioPlayerLocator.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\MessageHandler.h>
#include <Audio\AudioPlayer.h>

Enemy::Enemy(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Character(name, position, entityManager, entityID)
{}

void Enemy::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection)
{
	Character::handleEntityCollision(entity, intersection);
	if (entity->getName() == "Explosion")
	{
		if(m_lives <= 0)
		{
			AudioPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::EnemyDeath);
			GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::EnemyDeath);
		}
	}
}
