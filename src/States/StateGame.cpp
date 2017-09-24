#include <States\StateGame.h>
#include <Audio\AudioPlayer.h>
#include <Locators\AudioPlayerLocator.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\MessageHandler.h>
#include <Game\GameEvent.h>
#include <Game\GameLogic.h>
#include <Managers\StateManager.h>
#include <Locators\EventManagerLocator.h>
#include <iostream>
#include <Managers\EventManager.h>

StateGame::StateGame(StateManager& stateManager, StateType stateType)
	: StateBase(stateManager, stateType),
	m_collidableBoxSpawner(m_entityManager),
	m_gameManager(),
	m_animationNameConverter(),
	m_animationDetailsManager(),
	m_tileSheetManager(),
	m_entityManager(),
	m_levelManager(m_entityManager),
	m_gamePaused(false)
{
	m_gui.addButton(sf::Vector2f(250, 300), sf::Vector2f(50, 20), "Pause", "Pause");
	m_gui.addText(sf::Vector2f(75, 10), "Game Timer: " + std::to_string(m_gameManager.getGameTimer().getElaspedTime()), "GameTimer", 16);
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&StateGame::pauseGame, this), "StateGame", GameEvent::Pause);
	gameEventMessenger.subscribe(std::bind(&StateGame::unpauseGame, this), "StateGame", GameEvent::Unpause);
	gameEventMessenger.subscribe(std::bind(&StateGame::unpauseGame, this), "StateGame", GameEvent::StartedNewLevel);

	m_stateManager.createState(StateType::PauseMenu);
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
	StateBase::update(deltaTime);

	if (m_gamePaused)
	{
		return;
	}

	m_entityManager.update(deltaTime);
	m_gameManager.update(deltaTime);
	m_collidableBoxSpawner.update(deltaTime);

	m_gui.updateText(std::string("Game Timer: " + std::to_string(m_gameManager.getGameTimer().getElaspedTime())), "GameTimer");
}

void StateGame::draw(sf::RenderWindow& window)
{
	m_levelManager.draw(window);
	m_entityManager.draw(window);

	StateBase::draw(window);
}

void StateGame::pauseGame()
{
	m_gamePaused = true;
}

void StateGame::unpauseGame()
{
	m_gamePaused = false;
}

void StateGame::activateButton(const std::string& name)
{
	if (name == "Pause")
	{
		m_stateManager.switchToState(StateType::PauseMenu);
		pauseGame();
	}
}

void StateGame::printMessage()
{
	std::cout << "Hello World\n";
}