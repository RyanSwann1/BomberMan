#include "Entity.h"
#include "CollisionHandler.h"
#include <Tile\TileSheet.h>
#include <Managers\LevelManager.h>
#include <Locators\LevelManagerLocator.h>
#include <Game\DebugOverlay.h>
#include <math.h>

Entity::Entity(const std::string& name, EntityTag tag, const sf::Vector2f& spawnPosition, 
	EntityManager& entityManager, int ID, bool collidable)
	: m_position(spawnPosition),
	m_AABB(),
	m_animationPlayer(name),
	m_entityManager(entityManager),
	m_ID(ID),
	m_tag(tag),
	m_name(name),
	m_collidable(collidable)
{
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	m_AABB.width = tileSize / 2;
	m_AABB.height = tileSize / 2;

	updateAABB();
	m_animationPlayer.play(AnimationName::Default);
}

void Entity::draw(sf::RenderWindow & window)
{
	m_animationPlayer.draw(m_position, window);
}

void Entity::update(float deltaTime)
{
	m_animationPlayer.update(deltaTime);
	updateAABB();
}

void Entity::setPosition(float x, float y)
{
	m_position = sf::Vector2f(x, y);
}

void Entity::updateAABB()
{
	m_AABB.left = m_position.x + 4;
	m_AABB.top = m_position.y + 4;
}

bool Entity::isCollidable() const
{
	return m_collidable;
}

EntityTag Entity::getTag() const
{
	return m_tag;
}

const std::string& Entity::getName() const
{
	return m_name;
}

const sf::FloatRect & Entity::getAABB() const
{
	return m_AABB;
}

const sf::Vector2f & Entity::getPosition() const
{
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	//Determine which tile entity is on
	const sf::Vector2i tilePosition(m_position.x / tileSize, m_position.y / tileSize);

	return m_position;
}

int Entity::getID() const
{
	return m_ID;
}