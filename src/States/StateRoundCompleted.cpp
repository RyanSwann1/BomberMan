#include "StateRoundCompleted.h"
#include <Locators\AudioPlayerLocator.h>
#include <Audio\AudioPlayer.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Managers\StateManager.h>
#include <iostream>

StateRoundCompleted::StateRoundCompleted(StateManager& stateManager, StateType stateType)
	: StateBase(stateManager, stateType)
{
	auto& audioPlayer = AudioPlayerLocator::getAudioClipPlayer();
	audioPlayer.stopMusic();
	audioPlayer.playAudioClip(AudioClipName::Victory);
	GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::Pause);
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.broadcast(GameEvent::Pause);
	gameEventMessenger.subscribe(std::bind(&StateRoundCompleted::onWinGame, this), "StateRoundCompleted", GameEvent::WinGame);
	gameEventMessenger.subscribe(std::bind(&StateRoundCompleted::onPlayerDeath, this), "StateRoundCompleted", GameEvent::PlayerDeath);
	
	m_gui.addText(sf::Vector2f(30, 30), "Round Completed!", "TitleText", 30);
	m_gui.addButton(sf::Vector2f(175, 50), sf::Vector2f(100, 75), "Next Level", "NextLevel");
	m_gui.addButton(sf::Vector2f(50, 50), sf::Vector2f(100, 75), "Main Menu", "MainMenu");

}

StateRoundCompleted::~StateRoundCompleted()
{
	GameEventMessengerLocator::getGameEventMessenger().unsubscribe("StateRoundCompleted", GameEvent::WinGame);
	GameEventMessengerLocator::getGameEventMessenger().unsubscribe("StateRoundCompleted", GameEvent::PlayerDeath);
}

void StateRoundCompleted::activateButton(const std::string& name)
{
	if (name == "NextLevel")
	{
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::ChangeToNextLevel);
		m_stateManager.removeState(StateBase::getType());
	}
	else if (name == "MainMenu")
	{
		m_stateManager.switchToAndRemoveState(StateType::MainMenu, { StateType::Game, StateType::PauseMenu, StateBase::getType() });
	}
}

void StateRoundCompleted::onWinGame()
{
	m_stateManager.removeState(StateBase::getType());
}

void StateRoundCompleted::onPlayerDeath()
{
	m_stateManager.removeState(StateBase::getType());
}
