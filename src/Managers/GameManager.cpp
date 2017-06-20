#include <Managers\GameManager.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>

GameManager::GameManager()
	: m_enemiesRemaining(3)
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&GameManager::winGame, this), "GameManager", GameEvent::WinGame);
	gameEventMessenger.subscribe(std::bind(&GameManager::loseGame, this), "GameManager", GameEvent::PlayerDeath);
	gameEventMessenger.subscribe(std::bind(&GameManager::decreaseEnemyCount, this), "GameManager", GameEvent::EnemyDeath);
	gameEventMessenger.subscribe(std::bind(&GameManager::resetEnemyCount, this), "GameManager", GameEvent::ClearMap);
}

GameManager::~GameManager()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("GameManager", GameEvent::WinGame);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::PlayerDeath);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::EnemyDeath);
	gameEventMessenger.unsubscribe("GameManager", GameEvent::ClearMap);
}

void GameManager::winGame()
{

}

void GameManager::loseGame()
{
	
}

void GameManager::decreaseEnemyCount()
{
	--m_enemiesRemaining;
	if (!m_enemiesRemaining)
	{
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::ChangeToNextLevel);
	}
}

void GameManager::resetEnemyCount()
{
	m_enemiesRemaining = 3;
}
