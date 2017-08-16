#include <States\StateMainMenu.h>
#include <locators\FontManagerLocator.h>
#include <Managers\StateManager.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>

StateMainMenu::StateMainMenu(StateManager & stateManager, StateType type)
	: StateBase(stateManager, type)
{
	m_gui.addText(sf::Vector2f(75, 20), "Bomberman", 30);
	m_gui.addButton(sf::Vector2f(150, 50), sf::Vector2f(100, 75), "Play", GUIButtonName::StartGame);
	m_gui.addButton(sf::Vector2f(150, 150), sf::Vector2f(100, 75), "Exit", GUIButtonName::Exit);
}

void StateMainMenu::activateButton(GUIButtonName buttonName)
{
	switch (buttonName)
	{
	case GUIButtonName::StartGame :
	{
		m_stateManager.switchToAndRemoveState(StateType::Game, StateType::MainMenu);

		break;
	}
	case GUIButtonName::Exit :
	{
		auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
		gameEventMessenger.broadcast(GameEvent::CloseWindow);
		break;
	}
	}
}