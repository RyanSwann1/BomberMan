#include "Player.h"
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Managers\EntityManager.h>
#include <math.h>

Player::Player(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Character(name, position, entityManager, entityID)
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
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		placeBomb();
	}

	Character::update(deltaTime);
}

void Player::placeBomb()
{
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel().getTileSize();
	sf::Vector2f bombSpawnPosition;
	bombSpawnPosition.x = std::floor((m_position.x + tileSize / 2.0f) / tileSize);
	bombSpawnPosition.y = std::floor((m_position.y + tileSize / 2.0f) / tileSize);

	bombSpawnPosition.x *= tileSize;
	bombSpawnPosition.y *= tileSize;
	m_entityManager.addEntity("Bomb", bombSpawnPosition);
}