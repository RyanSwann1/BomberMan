#include "Entity.h"
#include <math.h>

Entity::Entity(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID)
	: m_entityManager(entityManager),
	m_animationPlayer(name),
	m_ID(entityID),
	m_position(position),
	m_speed(5, 5),
	m_maxVelocity(25, 25),
	m_friction(1, 1),
	m_currentMoveDirection(Direction::None)
{
	m_animationPlayer.play("Idle");
}

Entity::~Entity()
{
}

void Entity::draw(sf::RenderWindow & window)
{
	m_animationPlayer.draw(m_position, window);
}

void Entity::update(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_velocity.x -= m_speed.x;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_velocity.x += m_speed.x;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_velocity.y -= m_speed.y;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_velocity.y += m_speed.y;
	}

	handleVelocity();
	handleDirection();
	applyFriction();
	const sf::Vector2f deltaPos = m_position += (m_velocity * deltaTime);
	m_animationPlayer.update(deltaTime);
	m_animationPlayer.play(m_currentMoveDirection);
}

void Entity::handleVelocity()
{
	if (std::abs(m_velocity.x) > m_maxVelocity.x)
	{
		if (m_velocity.x > 0.0f)
		{
			m_velocity.x = m_maxVelocity.x;
		}
		else if(m_velocity.x < 0.0f)
		{
			m_velocity.x = -m_maxVelocity.x;
		}
	}
	else if (std::abs(m_velocity.y) > m_maxVelocity.y)
	{
		if (m_velocity.y > 0.0f)
		{
			m_velocity.y = m_maxVelocity.y;
		}
		else if (m_velocity.y < 0.0f)
		{
			m_velocity.y = -m_maxVelocity.y;
		}
	}
}

void Entity::handleDirection()
{
	//Moving horizontally
	if (std::abs(m_velocity.x) > std::abs(m_velocity.y))
	{
		if (m_velocity.x > 0.0f)
		{
			m_currentMoveDirection = Direction::Right;
		}
		else if(m_velocity.x < 0.0f)
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

void Entity::applyFriction()
{
	if (std::abs(m_velocity.x) > 0.0f)
	{
		if (m_velocity.x > 0.0f)
		{
			m_velocity.x -= m_friction.x;
		}
		else if (m_velocity.x < 0.0f)
		{
			m_velocity.x += m_friction.x;
		}
	}

	if (std::abs(m_velocity.y) > 0.0f)
	{
		if (m_velocity.y > 0.0f)
		{
			m_velocity.y -= m_friction.y;
		}
		else if (m_velocity.y < 0.0f)
		{
			m_velocity.y += m_friction.y;
		}
	}
}
