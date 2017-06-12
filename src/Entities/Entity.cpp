#include "Entity.h"
#include "CollisionHandler.h"
#include <Tile\TileSheet.h>
#include <Managers\LevelManager.h>
#include <math.h>

Entity::Entity(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID)
	: m_entityManager(entityManager),
	m_animationPlayer(name),
	m_ID(entityID),
	m_position(position),
	m_speed(50, 50),
	m_currentMoveDirection(Direction::None)
{
}

void Entity::draw(sf::RenderWindow & window)
{
	m_animationPlayer.draw(m_position, window);
}

void Entity::update(float deltaTime)
{
	handleDirection(m_velocity);
	CollisionHandler::ClampMovement(m_velocity, m_position);
	m_position += m_velocity;
	m_animationPlayer.update(deltaTime);
	m_velocity.x = 0;
	m_velocity.y = 0;
}

int Entity::getID() const
{
	return m_ID;
}

Direction Entity::getCurrentMoveDirection() const
{
	return m_currentMoveDirection;
}

void Entity::handleDirection(const sf::Vector2f& movement)
{
	//Moving horizontally
	if (std::abs(movement.x) > std::abs(movement.y))
	{
		if (movement.x > 0.0f)
		{
			m_currentMoveDirection = Direction::Right;
		}
		else if(movement.x < 0.0f)
		{
			m_currentMoveDirection = Direction::Left;
		}
	}
	//Moving vertically
	else if (std::abs(movement.y) > std::abs(movement.x))
	{
		if (movement.y > 0.0f)
		{
			m_currentMoveDirection = Direction::Down;
		}
		else if (movement.y < 0.0f)
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