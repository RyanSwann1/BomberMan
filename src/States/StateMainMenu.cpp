#include <States\StateMainMenu.h>
#include <locators\FontManagerLocator.h>
#include <Managers\StateManager.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>

StateMainMenu::StateMainMenu(StateManager & stateManager, StateType type)
	: StateBase(stateManager, type)
{
	m_gui.addText(sf::Vector2f(75, 20), "Bomberman", "TileText", 30);
	m_gui.addButton(sf::Vector2f(150, 50), sf::Vector2f(100, 75), "Play", "StartGame");
	m_gui.addButton(sf::Vector2f(150, 150), sf::Vector2f(100, 75), "Exit", "Exit");
}

void StateMainMenu::activateButton(const std::string& name)
{
	if (name == "StartGame")
	{
		m_stateManager.switchToAndRemoveState(StateType::Game, StateType::MainMenu);
	}
	else if (name == "Exit")
	{
		auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
		gameEventMessenger.broadcast(GameEvent::CloseWindow);
	}
}