#include <States\StatePauseMenu.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Managers\StateManager.h>

StatePauseMenu::StatePauseMenu(StateManager & stateManager, StateType type)
	: StateBase(stateManager, type)
{


	m_gui.addButton(sf::Vector2f(175, 50), sf::Vector2f(100, 75), "Resume", "Resume");
	m_gui.addButton(sf::Vector2f(50, 50), sf::Vector2f(100, 75), "Main Menu", "MainMenu");
}

void StatePauseMenu::activateButton(const std::string& name)
{
	if (name == "Resume")
	{
		m_stateManager.switchToState(StateType::Game);
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::Unpause);
	}
	else if (name == "MainMenu")
	{
		m_stateManager.switchToAndRemoveState(StateType::MainMenu, { StateType::Game, StateType::PauseMenu, StateBase::getType() });
	}
}
