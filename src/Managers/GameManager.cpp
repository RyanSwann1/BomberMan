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
#include <Locators\CollidableBoxSpawnerLocator.h>
#include <Game\CollidableBoxSpawner.h>
#include <Game\EntityMessenger.h>
#include <Game\GameLogic.h>
#include <GUI\GUI.h>
#include <iostream>

GameManager::GameManager()
	: m_maxEnemies(3),
	m_enemiesRemaining(0),
	m_gameTimer(0.0f, true, true, 80.0f),
	m_startCollidableBoxesTime(45.0f)
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&GameManager::onWinGame, this), "GameManager", GameEvent::WinGame);
	gameEventMessenger.subscribe(std::bind(&GameManager::onEnemyDeath, this), "GameManager", GameEvent::EnemyDeath);
	gameEventMessenger.subscribe(std::bind(&GameManager::onPlayerDeath, this), "GameManager", GameEvent::PlayerDeath);
	gameEventMessenger.subscribe(std::bind(&GameManager::onEnemySpawn, this), "GameManager", GameEvent::EnemySpawned);
	gameEventMessenger.subscribe(std::bind(&GameManager::onLevelReload, this), "GameManager", GameEvent::ReloadCurrentLevel);
	gameEventMessenger.subscribe(std::bind(&GameManager::onLevelReload, this), "GameManager", GameEvent::ClearMap);
}

GameManager::~GameManager()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("GameManager", GameEvent::WinGame);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::EnemyDeath);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::PlayerDeath);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::EnemySpawned);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::ReloadCurrentLevel);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::ClearMap);
}

const Timer & GameManager::getGameTimer() const
{
	return m_gameTimer;
}

void GameManager::update(float deltaTime)
{
	m_gameTimer.update(deltaTime);
	handleGameTimer();
	if (m_gameTimer.isExpired())
	{
		m_gameTimer.reset();
		m_gameTimer.deactivate();
	}
}

void GameManager::onWinGame()
{
	getStateManager().switchToAndRemoveState(StateType::GameCompleted, { StateType::Game, StateType::PauseMenu });
}

void GameManager::onEnemyDeath()
{
	--m_enemiesRemaining;

	if (!m_enemiesRemaining && GameLogic::isPlayerAlive())
	{
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::ChangeToNextLevel);
		return;
	}
	
	auto& entityMessenger = EntityMessengerLocator::getEntityMessenger();
	EntityMessage entityMessage(EntityEvent::TurnAggressive, 1);
	entityMessenger.broadcast(entityMessage);
}

void GameManager::onPlayerDeath()
{
	getStateManager().switchToState(StateType::RoundFailed);
}

void GameManager::onEnemySpawn()
{
	assert(m_enemiesRemaining < m_maxEnemies);
	++m_enemiesRemaining;

	if (m_enemiesRemaining < m_maxEnemies)
	{
		return;
	}

	bool isEnemyAggressive = false;
	for (const auto& entity : EntityManagerLocator::getEntityManager().getEntities())
	{
		if (entity->getTag() == EntityTag::Enemy 
			&& static_cast<Enemy*>(entity.get())->getType() == EnemyType::Aggressive)
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
	m_gameTimer.reset();
} 

void GameManager::handleGameTimer()
{
	if (!CollidableBoxSpawnerLocator::getCollidableBoxSpawner().isActive())
	{
		if (m_gameTimer.getElaspedTime() < m_startCollidableBoxesTime)
		{
			GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::SpawnCollidableBoxes);
		}
	}

	else if (m_gameTimer.getElaspedTime() <= 0.0f)
	{
		getStateManager().switchToState(StateType::RoundFailed);
	}
}