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

GameManager::GameManager()
	: m_enemiesRemaining(3)
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
