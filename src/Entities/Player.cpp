#include "Player.h"
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Managers\EntityManager.h>
#include <math.h>

Player::Player(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Entity(name, position, entityManager, entityID)
{
	m_animationPlayer.play("Idle");
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
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		placeBomb();
	}

	Entity::update(deltaTime);
	m_animationPlayer.play(Entity::getCurrentMoveDirection());
}

void Player::placeBomb()
{
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel().getTileSize();

	sf::Vector2f bombSpawnPosition;
	bombSpawnPosition.x = std::floor(m_position.x / tileSize);
	bombSpawnPosition.y = std::floor(m_position.y / tileSize);

	bombSpawnPosition.x *= tileSize;
	bombSpawnPosition.y *= tileSize;
	m_entityManager.addEntity("Bomb", bombSpawnPosition);
}