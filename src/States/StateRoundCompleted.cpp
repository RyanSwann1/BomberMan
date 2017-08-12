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
}

void StateRoundCompleted::draw(sf::RenderWindow & window)
{

}

void StateRoundCompleted::update(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::ChangeToNextLevel);
		m_stateManager.switchToAndRemoveState(StateType::Game, StateBase::getType());
	}
}
