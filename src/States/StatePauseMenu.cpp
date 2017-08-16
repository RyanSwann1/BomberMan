#include <States\StatePauseMenu.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Managers\StateManager.h>

StatePauseMenu::StatePauseMenu(StateManager & stateManager, StateType type)
	: StateBase(stateManager, type)
{
	m_gui.addButton(sf::Vector2f(175, 50), sf::Vector2f(100, 75), "Resume", GUIButtonName::Resume);
	m_gui.addButton(sf::Vector2f(50, 50), sf::Vector2f(100, 75), "Main Menu", GUIButtonName::MainMenu);
}

void StatePauseMenu::activateButton(GUIButtonName buttonName)
{
	switch (buttonName)
	{
	case GUIButtonName::Resume :
	{
		m_stateManager.switchToState(StateType::Game);
		break;
	}
	case GUIButtonName::MainMenu :
	{
		m_stateManager.switchToAndRemoveState(StateType::MainMenu, { StateType::Game, StateType::PauseMenu, StateBase::getType() });
		break;
	}
	}
}
