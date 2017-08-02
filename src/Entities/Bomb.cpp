#include "Bomb.h"
#include <Managers\EntityManager.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Game\GameLogic.h>
#include "CollisionHandler.h"
#include <Locators\AudioPlayerLocator.h>
#include <Audio\AudioPlayer.h>

Bomb::Bomb(const std::string& name, EntityTag tag, const sf::Vector2f & position, EntityManager & entityManager, int entityID, bool collidable)
	: Entity(name, tag, position, entityManager, entityID, collidable)
{}

void Bomb::update(float deltaTime)
{
	Entity::update(deltaTime);
	CollisionHandler::handleCollisions(m_position, m_entityManager, sf::Vector2f(), *this);

	if (m_animationPlayer.getCurrentAnimation(AnimationName::Default).getName() == AnimationName::Default 
		&& m_animationPlayer.getCurrentAnimation(AnimationName::Default).isFinished())
	{
		explode();
	}
}

void Bomb::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection)
{
	if (entity->getTag() == EntityTag::Explosion)
	{
		explode();
	}
}

void Bomb::explode()
{	
	for (const auto& spawnPosition : getExplosionSpawnPositions())
	{
		m_entityManager.addEntity("Explosion", spawnPosition);
		AudioPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::BombExplode);
	}

	m_entityManager.removeEntity(Entity::getID());
}

std::vector<sf::Vector2f> Bomb::getExplosionSpawnPositions() const
{
	std::vector<sf::Vector2f> explosionSpawnPositions;
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();

	//Get X axis spawn positions
	const int entityXPosition(std::floor(m_position.x / tileSize));
	const int entityYPosition(std::floor(m_position.y / tileSize));
	for (int x = entityXPosition; x <= entityXPosition + 2; ++x)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(x, entityYPosition), m_entityManager, EntityTag::Crate, tileSize))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			break;
		}

		if (!CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(x, entityYPosition), tileSize))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			continue;
		}

		break;
	}

	for (int x = entityXPosition; x >= entityXPosition - 2; --x)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(x, entityYPosition), m_entityManager, EntityTag::Crate, tileSize))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			break;
		}

		if (!CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(x, entityYPosition), tileSize))
		{
			explosionSpawnPositions.emplace_back(x * tileSize, entityYPosition * tileSize);
			continue;
		}

		break;
	}

	//Get Y Axis spawn positions
	for (int y = entityYPosition; y <= entityYPosition + 2; ++y)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(entityXPosition, y), m_entityManager, EntityTag::Crate, tileSize))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			break;
		}

		if (!CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(entityXPosition, y), tileSize))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			continue;
		}

		break;
	}

	for (int y = entityYPosition; y >= entityYPosition - 2; --y)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(entityXPosition, y), m_entityManager, EntityTag::Crate, tileSize))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			break;
		}

		if (!CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(entityXPosition, y), tileSize))
		{
			explosionSpawnPositions.emplace_back(entityXPosition * tileSize, y * tileSize);
			continue;
		}

		break;
	}

	return explosionSpawnPositions;
}