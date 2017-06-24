#include "Player.h"
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Audio\AudioPlayer.h>
#include <Locators\AudioClipPlayerLocator.h>
#include <Audio\AudioClipName.h>
#include <math.h>

Player::Player(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Character(name, position, entityManager, entityID)
{}

void Player::update(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_velocity.x -= m_speed.x * deltaTime;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_velocity.x += m_speed.x * deltaTime;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_velocity.y -= m_speed.y * deltaTime;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_velocity.y += m_speed.y * deltaTime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (m_bombPlacementTimer.isExpired())
		{
			AudioClipPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::PlaceBomb);
			placeBomb();
		}
	}
	
	Character::update(deltaTime);
}

void Player::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect & intersection)
{
	Character::handleEntityCollision(entity, intersection);

	if (entity->getName() == "Explosion")
	{
		if (m_lives <= 0)
		{
			AudioClipPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::PlayerDeath);
			GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::ReloadCurrentLevel);
		}
	}
}