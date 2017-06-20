#include "Character.h"
#include "CollisionHandler.h"
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Managers\EntityManager.h>

Character::Character(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Entity(name, position, entityManager, entityID),
	m_speed(50, 50),
	m_currentMoveDirection(Direction::None),
	m_bombPlacementTimer(1.25f),
	m_lives(0)
{}

void Character::update(float deltaTime)
{
	m_bombPlacementTimer.update(deltaTime);
	handleAnimation();
	handleDirection();
	CollisionHandler::handleCollisions(m_position, m_entityManager, m_velocity, *this);
	m_position += m_velocity;

	m_velocity.x = 0;
	m_velocity.y = 0;

	Entity::update(deltaTime);
}

void Character::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection)
{
	if (entity->getName() == "Crate")
	{
		if (m_velocity.x < 0.f)
		{
			m_velocity.x += intersection.width;
		}
		else if (m_velocity.x > 0.f)
		{
			m_velocity.x -= intersection.width;
		}
		if (m_velocity.y < 0.f)
		{
			m_velocity.y += intersection.height;
		}
		else if (m_velocity.y > 0.f)
		{
			m_velocity.y -= intersection.height;
		}
	}
	else if (entity->getName() == "Explosion")
	{
		--m_lives;
		if (m_lives <= 0)
		{
			m_entityManager.removeEntity(Entity::getID());
		}
	}
}

void Character::handleDirection()
{
	//Moving horizontally
	if (std::abs(m_velocity.x) > std::abs(m_velocity.y))
	{
		if (m_velocity.x > 0.0f)
		{
			m_currentMoveDirection = Direction::Right;
		}
		else if (m_velocity.x < 0.0f)
		{
			m_currentMoveDirection = Direction::Left;
		}
	}
	//Moving vertically
	else if (std::abs(m_velocity.y) > std::abs(m_velocity.x))
	{
		if (m_velocity.y > 0.0f)
		{
			m_currentMoveDirection = Direction::Down;
		}
		else if (m_velocity.y < 0.0f)
		{
			m_currentMoveDirection = Direction::Up;
		}
	}
	//Idling
	else
	{
		m_currentMoveDirection = Direction::None;
	}
}

void Character::handleAnimation()
{
	switch (m_currentMoveDirection)
	{
	case Direction::Right:
	{
		m_animationPlayer.play("WalkingRight", Direction::Right);
		break;
	}
	case Direction::Left:
	{
		m_animationPlayer.play("WalkingLeft", Direction::Left);
		break;
	}
	case Direction::Up:
	{
		m_animationPlayer.play("WalkingUp", Direction::Up);
		break;
	}
	case Direction::Down:
	{
		m_animationPlayer.play("WalkingDown", Direction::Down);
		break;
	}
	case Direction::None:
	{
		m_animationPlayer.play("Default");
		break;
	}
	}
}

void Character::placeBomb()
{
	if (!m_bombPlacementTimer.isExpired())
	{
		return;
	}

	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel().getTileSize();
	sf::Vector2f bombSpawnPosition;
	bombSpawnPosition.x = std::floor((m_position.x + tileSize / 2.0f) / tileSize);
	bombSpawnPosition.y = std::floor((m_position.y + tileSize / 2.0f) / tileSize);

	bombSpawnPosition.x *= tileSize;
	bombSpawnPosition.y *= tileSize;
	m_entityManager.addEntity("Bomb", bombSpawnPosition);
	m_bombPlacementTimer.reset();
}
