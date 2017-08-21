#include "Player.h"
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Audio\AudioPlayer.h>
#include <Locators\AudioPlayerLocator.h>
#include <Audio\AudioClipName.h>
#include <math.h>

Player::Player(const std::string& name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID, bool collidable)
	: BombCarrier(name, tag, spawnPosition, entityManager, ID, collidable)
{
}

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
		placeBomb();	
	}
	
	BombCarrier::update(deltaTime);
}

void Player::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect & intersection)
{
	if (!m_lives)
	{
		return;
	}
	Character::handleEntityCollision(entity, intersection);

	if (m_lives <= 0)
	{
		AudioPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::PlayerDeath);
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::PlayerDeath);
	}

	if (entity->getTag() == EntityTag::PowerUp)
	{
		handlePowerUpCollision(entity);
	}
}

void Player::handlePowerUpCollision(const std::unique_ptr<Entity>& entity)
{
	if (entity->getName() == "PowerUpSpeedBoost")
	{
		Character::increaseSpeed(2.5f, 2.5f);
	}
	else if (entity->getName() == "PowerUpIncreaseBomb")
	{
		BombCarrier::increaseBombQuantity();
	}
	else if (entity->getName() == "PowerUpIncreaseBombPower")
	{
		BombCarrier::increaseBombPower();
	}
}
