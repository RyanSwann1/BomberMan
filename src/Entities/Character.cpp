#include "Character.h"
#include "CollisionHandler.h"

Character::Character(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Entity(name, position, entityManager, entityID),
	m_speed(50, 50),
	m_currentMoveDirection(Direction::None)
{}

void Character::update(float deltaTime)
{
	handleAnimation();
	handleDirection();
	CollisionHandler::ClampMovement(m_velocity, m_position);
	m_position += m_velocity;
	m_velocity.x = 0;
	m_velocity.y = 0;

	Entity::update(deltaTime);
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