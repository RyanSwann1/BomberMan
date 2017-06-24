#include <States\StateGame.h>
#include <Audio\AudioPlayer.h>
#include <Locators\AudioClipPlayerLocator.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\MessageHandler.h>
#include <Game\GameEvent.h>
#include <iostream>

StateGame::StateGame(StateType stateType)
	: StateBase(stateType),
	m_tileSheetManager(),
	m_entityManager(),
	m_levelManager(),
	m_gameManager()
{
	std::cout << "You have entered the game.\n";
	AudioClipPlayerLocator::getAudioClipPlayer().playMusic("LevelMusic.wav");
}

void StateGame::update(float deltaTime)
{
	m_entityManager.update(deltaTime);
	m_levelManager.update();
}

void StateGame::draw(sf::RenderWindow& window)
{
	m_levelManager.draw(window);
	m_entityManager.draw(window);
}