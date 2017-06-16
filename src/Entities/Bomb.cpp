#include "Bomb.h"
#include <Managers\EntityManager.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Game\GameLogic.h>

Bomb::Bomb(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Entity(name, position, entityManager, entityID)
{
	m_animationPlayer.play("Default");
	
}

void Bomb::update(float deltaTime)
{
	Entity::update(deltaTime);
	if (m_animationPlayer.getCurrentAnimation("Default").getName() == "Default" && m_animationPlayer.getCurrentAnimation("Default").isFinished())
	{
		const auto explosionSpawnPositions = GameLogic::explosionSpawnPositions(m_position, LevelManagerLocator::getLevelManager().getCurrentLevel());
		for (const auto& spawnPosition : explosionSpawnPositions)
		{
			m_entityManager.addEntity("Explosion", spawnPosition);
		}

		m_entityManager.removeEntity(Entity::getID());
	}
}