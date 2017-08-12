#include <States\StateGame.h>
#include <Audio\AudioPlayer.h>
#include <Locators\AudioPlayerLocator.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\MessageHandler.h>
#include <Game\GameEvent.h>
#include <Game\GameLogic.h>
#include <Managers\StateManager.h>

StateGame::StateGame(StateManager& stateManager, StateType stateType)
	: StateBase(stateManager, stateType),
	m_gameManager(m_entityManager),
	m_animationNameConverter(),
	m_animationDetailsManager(),
	m_tileSheetManager(),
	m_entityManager(),
	m_levelManager(m_entityManager),
	m_gamePaused(false)
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&StateGame::pauseGame, this), "StateGame", GameEvent::Pause);
	gameEventMessenger.subscribe(std::bind(&StateGame::unpauseGame, this), "StateGame", GameEvent::Unpause);
	gameEventMessenger.subscribe(std::bind(&StateGame::onNewLevel, this), "StateGame", GameEvent::StartedNewLevel);
}

StateGame::~StateGame()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("StateGame", GameEvent::Pause);
	gameEventMessenger.unsubscribe("StateGame", GameEvent::Unpause);
	gameEventMessenger.unsubscribe("StateGame", GameEvent::StartedNewLevel);
}

void StateGame::update(float deltaTime)
{
	if (m_gamePaused)
	{
		return;
	}

	m_entityManager.update(deltaTime);
	m_gameManager.update(deltaTime);
}

void StateGame::draw(sf::RenderWindow& window)
{
	m_levelManager.draw(window);
	m_entityManager.draw(window);
}

void StateGame::pauseGame()
{
	m_gamePaused = true;
}

void StateGame::unpauseGame()
{
	m_gamePaused = false;
}

void StateGame::onNewLevel()
{
	m_gamePaused = false;
}
