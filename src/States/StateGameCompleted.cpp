#include "StateGameCompleted.h"
#include <Managers\StateManager.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>

StateGameCompleted::StateGameCompleted(StateManager& stateManager, StateType stateType)
	: StateBase(stateManager, stateType)
{
	m_gui.addText(sf::Vector2f(75, 20), "You Win!", "TitleText", 30);
	m_gui.addButton(sf::Vector2f(150, 150), sf::Vector2f(100, 75), "MainMenu", "MainMenu");
	GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::WinStateEntered);
}

void StateGameCompleted::activateButton(const std::string& name)
{
	if (name == "MainMenu")
	{
		m_stateManager.switchToAndRemoveState(StateType::MainMenu, StateBase::getType());
	}
}