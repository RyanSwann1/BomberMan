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
	m_speed(75, 75),
	m_currentMoveDirection(Direction::None)
{
	m_animationPlayer.play(m_currentMoveDirection);
}

void Entity::draw(sf::RenderWindow & window)
{
	m_animationPlayer.draw(m_position, window);
}

void Entity::update(float deltaTime)
{
	sf::Vector2f movement;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		movement.x -= m_speed.x * deltaTime;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		movement.x += m_speed.x * deltaTime;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		movement.y -= m_speed.y * deltaTime;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		movement.y += m_speed.y * deltaTime;
	}

	handleDirection(movement);
	CollisionHandler::ClampMovement(movement, m_position, LevelManagerLocator::getLevelManager().getCurrentLevel());
	m_position += movement;
	m_animationPlayer.update(deltaTime);
	m_animationPlayer.play(m_currentMoveDirection);
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