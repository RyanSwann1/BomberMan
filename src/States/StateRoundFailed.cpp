#include <States\StateRoundFailed.h>
#include <Managers\StateManager.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\MessageHandler.h>
#include <iostream>
#include <Game\GameEvent.h>

StateRoundFailed::StateRoundFailed(StateManager & stateManager, StateType stateType)
	: StateBase(stateManager, stateType)
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.broadcast(GameEvent::Pause);
	gameEventMessenger.subscribe(std::bind(&StateRoundFailed::onEnteringWinState, this), "StateRoundFailed", GameEvent::WinStateEntered);

	m_gui.addText(sf::Vector2f(75, 20), "You Died!", "TitleText", 30);
	m_gui.addButton(sf::Vector2f(150, 50), sf::Vector2f(100, 75), "Retry", "Retry");
	m_gui.addButton(sf::Vector2f(150, 150), sf::Vector2f(100, 75), "MainMenu", "MainMenu");
	std::cout << "State Lost Game\n";
}

StateRoundFailed::~StateRoundFailed()
{
	GameEventMessengerLocator::getGameEventMessenger().unsubscribe("StateRoundFailed", GameEvent::WinStateEntered);
}

void StateRoundFailed::activateButton(const std::string& name)
{
	if (name == "Retry")
	{
		auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
		gameEventMessenger.broadcast(GameEvent::ReloadCurrentLevel);
		m_stateManager.switchToAndRemoveState(StateType::Game, StateType::RoundFailed);
	}
	else if (name == "MainMenu")
	{
		m_stateManager.switchToAndRemoveState(StateType::MainMenu, { StateType::Game, StateType::PauseMenu, StateBase::getType() });
	}
}

void StateRoundFailed::onEnteringWinState()
{
	m_stateManager.removeState(StateBase::getType());
}