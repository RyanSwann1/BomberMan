#include <States\StateRoundFailed.h>
#include <Managers\StateManager.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\MessageHandler.h>
#include <iostream>
#include <Game\GameEvent.h>

StateRoundFailed::StateRoundFailed(StateManager & stateManager, StateType stateType)
	: StateBase(stateManager, stateType)
{
	GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::Pause);
	m_gui.addText(sf::Vector2f(75, 20), "You Died!", 30);
	m_gui.addButton(sf::Vector2f(150, 50), sf::Vector2f(100, 75), "Retry", GUIButtonName::Retry);
	m_gui.addButton(sf::Vector2f(150, 150), sf::Vector2f(100, 75), "MainMenu", GUIButtonName::MainMenu);
}

void StateRoundFailed::activateButton(GUIButtonName buttonName)
{
	switch (buttonName)
	{
	case GUIButtonName::Retry :
	{
		auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
		gameEventMessenger.broadcast(GameEvent::ReloadCurrentLevel);
		m_stateManager.switchToAndRemoveState(StateType::Game, StateType::RoundFailed);
		break;
	}
	case GUIButtonName::MainMenu :
	{
		m_stateManager.switchToAndRemoveState(StateType::MainMenu, { StateType::Game, StateType::PauseMenu, StateBase::getType() });
		break;
	}
	}
}
