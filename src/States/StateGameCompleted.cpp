#include "StateGameCompleted.h"
#include <Managers\StateManager.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>

StateGameCompleted::StateGameCompleted(StateManager& stateManager, StateType stateType)
	: StateBase(stateManager, stateType)
{
	m_gui.addText(sf::Vector2f(75, 20), "You Died!", 30);
	m_gui.addButton(sf::Vector2f(150, 50), sf::Vector2f(100, 75), "Retry", GUIButtonName::Retry);
	m_gui.addButton(sf::Vector2f(150, 150), sf::Vector2f(100, 75), "MainMenu", GUIButtonName::MainMenu);
}

void StateGameCompleted::activateButton(GUIButtonName buttonName)
{
	switch (buttonName)
	{
	case GUIButtonName::NextLevel :
	{
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::ChangeToNextLevel);
		m_stateManager.switchToAndRemoveState(StateType::Game, StateBase::getType());
		break;
	}
	case GUIButtonName::MainMenu :
	{
		m_stateManager.switchToAndRemoveState(StateType::MainMenu, { StateType::Game, StateType::PauseMenu, StateBase::getType() });
		break;
	}
	}
}
