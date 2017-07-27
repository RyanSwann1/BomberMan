#include "Character.h"
#include "CollisionHandler.h"
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Managers\EntityManager.h>
#include <Locators\AudioPlayerLocator.h>
#include <Audio\AudioPlayer.h>

Character::Character(const std::string& name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID)
	: Entity(name, tag ,spawnPosition, entityManager, ID),
	m_speed(50, 50),
	m_velocity(),
	m_oldPosition(),
	m_currentMoveDirection(Direction::None),
	m_bombPlacementTimer(0.5f, true),
	m_lives(1)
{}

void Character::update(float deltaTime)
{
	m_bombPlacementTimer.update(deltaTime);
	handleAnimation();
	handleDirection();
	CollisionHandler::handleCollisions(m_position, m_entityManager, m_velocity, *this);
	m_oldPosition = m_position;
	m_position += m_velocity;

	m_velocity.x = 0;
	m_velocity.y = 0;

	Entity::update(deltaTime);
}

void Character::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection)
{
	if (entity->getTag() == EntityTag::Crate)
	{
		CollisionHandler::clampMovement(intersection, m_velocity);
	}
	else if (entity->getTag() == EntityTag::Explosion)
	{
		--m_lives;
		if (!m_lives)
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
		m_animationPlayer.play(AnimationName::WalkingRight, Direction::Right);
		break;
	}
	case Direction::Left:
	{
		m_animationPlayer.play(AnimationName::WalkingLeft, Direction::Left);
		break;
	}
	case Direction::Up:
	{
		m_animationPlayer.play(AnimationName::WalkingUp, Direction::Up);
		break;
	}	
	case Direction::Down:
	{
		m_animationPlayer.play(AnimationName::WalkingDown, Direction::Down);
		break;
	}
	case Direction::None:
	{
		m_animationPlayer.play(AnimationName::Default);
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

	AudioPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::PlaceBomb);
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	sf::Vector2f bombSpawnPosition;
	bombSpawnPosition.x = std::floor((m_position.x + tileSize / 2.0f) / tileSize);
	bombSpawnPosition.y = std::floor((m_position.y + tileSize / 2.0f) / tileSize);

	bombSpawnPosition.x *= tileSize;
	bombSpawnPosition.y *= tileSize;
	m_entityManager.addEntity("Bomb", bombSpawnPosition);
	m_bombPlacementTimer.reset();
}