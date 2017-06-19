#include "Bomb.h"
#include <Managers\EntityManager.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Game\GameLogic.h>
#include "CollisionHandler.h"
#include <Locators\AudioClipPlayerLocator.h>
#include <Audio\AudioClipPlayer.h>

Bomb::Bomb(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Entity(name, position, entityManager, entityID)
{}

void Bomb::update(float deltaTime)
{
	Entity::update(deltaTime);
	CollisionHandler::handleCollisions(m_position, m_entityManager, sf::Vector2f(), *this);

	if (m_animationPlayer.getCurrentAnimation("Default").getName() == "Default" && m_animationPlayer.getCurrentAnimation("Default").isFinished())
	{
		explode();
	}
}

void Bomb::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection)
{
	if (entity->getName() == "Explosion")
	{
		explode();
	}
}

void Bomb::explode()
{
	const auto explosionSpawnPositions = GameLogic::explosionSpawnPositions(m_position, LevelManagerLocator::getLevelManager().getCurrentLevel());
	for (const auto& spawnPosition : explosionSpawnPositions)
	{
		m_entityManager.addEntity("Explosion", spawnPosition);
		AudioClipPlayerLocator::getAudioClipPlayer().playSound(AudioClipName::BombExplode);
	}

	m_entityManager.removeEntity(Entity::getID());
}