#include <Managers\GameManager.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Locators\EntityManagerLocator.h>
#include <Managers\EntityManager.h>
#include <Managers\StateManager.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Entities\CollisionHandler.h>
#include <Entities\Enemy.h>
#include <Game\GameEvent.h>
#include <Locators\EntityMessengerLocator.h>
#include <Game\EntityMessenger.h>

int getNextRowSpawn(const sf::Vector2i& startingPoint, const sf::Vector2i& endingPoint, Direction searchDirection, int tileSize);
int getNextColumnSpawn(const sf::Vector2i& startingPoint, const sf::Vector2i& endingPoint, Direction searchDirection, int tileSize);

GameManager::GameManager(EntityManager& entityManager)
	: m_maxEnemies(3),
	m_enemiesRemaining(0),
	m_entityManager(entityManager),
	m_totalGameTime(10.0f),
	m_gameTimer(m_totalGameTime, true),
	m_spawnTimer(0.25f, true),
	m_reduceMapSize(false),
	m_currentSpawnPosition(0, 0),
	m_spawnDirection(Direction::Right),
	m_nextRoundTimer(2.0f, false)
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&GameManager::onWinGame, this), "GameManager", GameEvent::WinGame);
	gameEventMessenger.subscribe(std::bind(&GameManager::onEnemyDeath, this), "GameManager", GameEvent::EnemyDeath);
	gameEventMessenger.subscribe(std::bind(&GameManager::onPlayerDeath, this), "GameManager", GameEvent::PlayerDeath);
	gameEventMessenger.subscribe(std::bind(&GameManager::onEnemySpawn, this), "GameManager", GameEvent::EnemySpawned);
	gameEventMessenger.subscribe(std::bind(&GameManager::onLevelReload, this), "GameManager", GameEvent::ReloadCurrentLevel);
}

GameManager::~GameManager()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("GameManager", GameEvent::WinGame);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::EnemyDeath);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::PlayerDeath);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::EnemySpawned);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::ReloadCurrentLevel);
}

void GameManager::increaseEntityBombExplosiveRadius(const std::unique_ptr<Entity>& entity, int entityID)
{

}

void GameManager::update(float deltaTime)
{
	m_nextRoundTimer.update(deltaTime);
	if (m_nextRoundTimer.isExpired())
	{
		getStateManager().switchToState(StateType::RoundCompleted);
		m_nextRoundTimer.reset();
		m_nextRoundTimer.deactivate();
	}
}

void GameManager::onWinGame()
{
	getStateManager().switchToAndRemoveState(StateType::GameCompleted, { StateType::Game, StateType::PauseMenu });
}

void GameManager::onEnemyDeath()
{
	--m_enemiesRemaining;
	if (!m_enemiesRemaining)
	{
		m_nextRoundTimer.activate();
		return;
	}

	auto& entityMessenger = EntityMessengerLocator::getEntityMessenger();
	EntityMessage entityMessage(EntityEvent::TurnAggressive, 1);
	entityMessenger.broadcast(entityMessage);

}

void GameManager::onPlayerDeath()
{
	getStateManager().switchToState(StateType::RoundFailed);
	m_nextRoundTimer.reset();
	m_nextRoundTimer.deactivate();
}

void GameManager::reduceMapSize()
{
	const auto& levelSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getSize();	
	switch (m_spawnDirection)
	{
	case Direction::Left :
	{
		if (m_currentSpawnPosition.x <= 0)
		{
			m_spawnDirection = Direction::Up;
			break;
		}

		--m_currentSpawnPosition.x;
		break;
	}
	case Direction::Right :
	{
		if (m_currentSpawnPosition.x == levelSize.x - 1)
		{
			m_spawnDirection = Direction::Down;
			break;
		}

		++m_currentSpawnPosition.x;
		break;
	}
	case Direction::Up :
	{
		

		break;
	}
	case Direction::Down :
	{
		if (m_currentSpawnPosition.y >= levelSize.y - 1)
		{
			m_spawnDirection = Direction::Left;
			break;
		}

		++m_currentSpawnPosition.y;
		break;
	}
	}

	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	m_entityManager.addEntity("CollidableTile", sf::Vector2f(m_currentSpawnPosition.x * tileSize, m_currentSpawnPosition.y * tileSize));
	m_spawnTimer.reset();
}

void GameManager::assignEnemyToAggressive()
{

}

void GameManager::onEnemySpawn()
{
	assert(m_enemiesRemaining < m_maxEnemies);
	++m_enemiesRemaining;

	if (m_enemiesRemaining != m_maxEnemies)
	{
		return;
	}

	bool isEnemyAggressive = false;
	for (const auto& entity : EntityManagerLocator::getEntityManager().getEntities())
	{
		if (entity->getTag() == EntityTag::Enemy && static_cast<Enemy*>(entity.get())->getType() == EnemyType::Aggressive)
		{
			isEnemyAggressive = true;
			break;
		}
	}

	if (!isEnemyAggressive)
	{
		EntityMessage entityMessage(EntityEvent::TurnAggressive, 1);
		auto& entityMessenger = EntityMessengerLocator::getEntityMessenger();
		entityMessenger.broadcast(entityMessage);
	}
}

void GameManager::onLevelReload()
{
	m_enemiesRemaining = 0;
}

int getNextRowSpawn(const sf::Vector2i& startingPoint, const sf::Vector2i& endingPoint, Direction searchDirection, int tileSize)
{
	bool rowFound = false;
	int i = 0;
	switch (searchDirection)
	{
	case Direction::Up :
	{
		for (int y = startingPoint.y; y >= endingPoint.y; --y)
		{
			if (!rowFound && CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(startingPoint.x, y), tileSize))
			{
				rowFound = true;
			}

			if (rowFound && !CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(startingPoint.x, y), tileSize))
			{
				i = y;
				break;
			}
		}
		break;
	}
	case Direction::Down :
	{
		for (int y = startingPoint.y; y <= endingPoint.y; ++y)
		{
			if (!rowFound && CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(startingPoint.x, y), tileSize))
			{
				rowFound = true;
			}

			if (rowFound && !CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(startingPoint.x, y), tileSize))
			{
				i = y;
				break;
			}
		}
		break;
	}
	}

	assert(i != 0);
	return i;
}

int getNextColumnSpawn(const sf::Vector2i & startingPoint, const sf::Vector2i & endingPoint, Direction searchDirection, int tileSize)
{
	bool columnFound = false;
	int i = 0;
	switch (searchDirection)
	{
	case Direction::Right :
	{
		for (int x = startingPoint.x; x <= endingPoint.x; ++x)
		{
			if (!columnFound && CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(x, startingPoint.y), tileSize))
			{
				columnFound = true;
			}

			if (columnFound && !CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(x, startingPoint.y), tileSize))
			{
				i = x;
				break;
			}
		}
		break;
	}
	case Direction::Left :
	{
		for (int x = startingPoint.x; x >= endingPoint.x; --x)
		{
			if (!columnFound && CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(x, startingPoint.y), tileSize))
			{
				columnFound = true;
			}

			if (columnFound && !CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(x, startingPoint.y), tileSize))
			{
				i = x;
				break;
			}
		}

		break;
	}
	}

	assert(i != 0);
	return i;
}
