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
	audioPlayer.playAudioClip(AudioClipName::Victory);
	audioPlayer.stopMusic();
	std::cout << "Press 'Space' for the next level" << "\n";
}

void StateRoundCompleted::draw(sf::RenderWindow & window)
{

}

void StateRoundCompleted::update(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::ChangeToNextLevel);
		m_stateManager.removeState(StateType::RoundCompleted);
		m_stateManager.switchToState(StateType::Game);
	}
}
