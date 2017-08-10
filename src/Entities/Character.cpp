#include "Character.h"
#include "CollisionHandler.h"
#include <Managers\EntityManager.h>


Character::Character(const std::string& name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID, bool collidable)
	: Entity(name, tag ,spawnPosition, entityManager, ID, collidable),
	m_maxSpeed(75, 75),
	m_speed(35, 35),
	m_velocity(),
	m_oldPosition(),
	m_currentMoveDirection(Direction::None),
	m_lives(1)
{}

void Character::update(float deltaTime)
{
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
	if (entity->isCollidable())
	{
		CollisionHandler::clampMovement(intersection, m_velocity);
	}

	switch (entity->getTag())
	{
	case EntityTag::Explosion :
	{
		--m_lives;
		if (!m_lives)
		{
			m_entityManager.removeEntity(Entity::getID());
		}
		break;
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

void Character::resetVelocity()
{
	m_velocity.x = 0;
	m_velocity.y = 0;
}

void Character::increaseSpeed(float x, float y)
{
	if (m_speed.x + x < m_maxSpeed.x)
	{
		m_speed.x += x;
	}

	if (m_speed.y + y < m_maxSpeed.y)
	{
		m_speed.y += y;
	}
}