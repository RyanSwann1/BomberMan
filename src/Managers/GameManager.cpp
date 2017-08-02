#include <Managers\GameManager.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Locators\EntityManagerLocator.h>
#include <Managers\EntityManager.h>
#include <Managers\StateManager.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Game\AILogic.h>
#include <Game\GameLogic.h>
#include <Game\DebugOverlay.h>
#include <Entities\CollisionHandler.h>
#include <Entities\Direction.h>

int getNextRowSpawn(const sf::Vector2i& startingPoint, const sf::Vector2i& endingPoint, Direction searchDirection, int tileSize);
int getNextColumnSpawn(const sf::Vector2i& startingPoint, const sf::Vector2i& endingPoint, Direction searchDirection, int tileSize);

GameManager::GameManager(EntityManager& entityManager)
	: m_enemiesRemaining(3),
	m_entityManager(entityManager),
	m_totalGameTime(10.0f),
	m_gameTimer(m_totalGameTime, true),
	m_spawnTimer(0.25f, true),
	m_reduceMapSize(false),
	m_currentSpawnPosition(0, 0),
	m_spawnDirection(Direction::Right)
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&GameManager::winGame, this), "GameManager", GameEvent::WinGame);
	gameEventMessenger.subscribe(std::bind(&GameManager::onEnemyDeath, this), "GameManager", GameEvent::EnemyDeath);
	gameEventMessenger.subscribe(std::bind(&GameManager::resetEnemyCount, this), "GameManager", GameEvent::ClearMap);
	gameEventMessenger.subscribe(std::bind(&GameManager::onPlayerDeath, this), "GameManager", GameEvent::PlayerDeath);
}

GameManager::~GameManager()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("GameManager", GameEvent::WinGame);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::EnemyDeath);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::ClearMap);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::PlayerDeath);
}

void GameManager::update(float deltaTime)
{
	/*m_spawnTimer.update(deltaTime);
	if (m_spawnTimer.isExpired())
	{
		reduceMapSize();
	}*/

	//m_gameTimer.update(deltaTime);
	//if (m_reduceMapSize)
	//{
	//	reduceMapSize();
	//}
	//else
	//{
	//	if (m_gameTimer.getElaspedTime() >= (m_totalGameTime / 2.0f))
	//	{
	//		m_reduceMapSize = true;
	//	}
	//}
}

void GameManager::winGame()
{
	getStateManager().switchToAndRemoveState(StateType::GameCompleted, StateType::Game);
}

void GameManager::onEnemyDeath()
{
	--m_enemiesRemaining;
	if (m_enemiesRemaining == 1)
	{
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::EnemyAggressive);
	}

	if (!m_enemiesRemaining)
	{
		getStateManager().switchToState(StateType::RoundCompleted);
	}
}

void GameManager::resetEnemyCount()
{
	m_enemiesRemaining = 3;
}

void GameManager::onPlayerDeath()
{
	resetEnemyCount();	
	getStateManager().switchToState(StateType::RoundFailed);
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
