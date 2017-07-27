#include <States\StateGame.h>
#include <Audio\AudioPlayer.h>
#include <Locators\AudioPlayerLocator.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\MessageHandler.h>
#include <Game\GameEvent.h>
#include <Game\GameLogic.h>
#include <iostream>

StateGame::StateGame(StateManager& stateManager, StateType stateType)
	: StateBase(stateManager, stateType),
	m_gameManager(),
	m_animationNameConverter(),
	m_animationDetailsManager(),
	m_tileSheetManager(),
	m_entityManager(),
	m_levelManager(m_entityManager)
{
	std::cout << "You have entered the game.\n";
}

void StateGame::update(float deltaTime)
{
	m_entityManager.update(deltaTime);
}

void StateGame::draw(sf::RenderWindow& window)
{
	m_levelManager.draw(window);
	m_entityManager.draw(window);
}