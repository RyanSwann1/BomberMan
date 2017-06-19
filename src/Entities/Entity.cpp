#include "Entity.h"
#include "CollisionHandler.h"
#include <Tile\TileSheet.h>
#include <Managers\LevelManager.h>
#include <Locators\LevelManagerLocator.h>
#include <math.h>

Entity::Entity(const std::string& entityName, const sf::Vector2f& entityPosition, EntityManager& entityManager, int entityID)
	: m_position(entityPosition),
	m_AABB(),
	m_animationPlayer(entityName),
	m_entityManager(entityManager),
	m_ID(entityID),
	m_name(entityName)
{
	const auto& level = LevelManagerLocator::getLevelManager().getCurrentLevel();
	const int tileSize = level.getTileSize();
	m_AABB.width = tileSize / 2;
	m_AABB.height = tileSize / 2;
	updateAABB();
	m_animationPlayer.play("Default");

	m_debugRect.setFillColor(sf::Color::Green);
}

void Entity::draw(sf::RenderWindow & window)
{
	m_animationPlayer.draw(m_position, window);
	//window.draw(m_debugRect);
}

void Entity::update(float deltaTime)
{
	m_animationPlayer.update(deltaTime);
	updateAABB();
}

void Entity::updateAABB()
{
	m_AABB.left = m_position.x + 4;
	m_AABB.top = m_position.y + 4;
	m_debugRect.setPosition(sf::Vector2f(m_AABB.left, m_AABB.top));
	m_debugRect.setSize(sf::Vector2f(m_AABB.width, m_AABB.height));
}

const std::string & Entity::getName() const
{
	return m_name;
}

const sf::FloatRect & Entity::getAABB() const
{
	return m_AABB;
}

const sf::Vector2f & Entity::getPosition() const
{
	return m_position;
}

int Entity::getID() const
{
	return m_ID;
}