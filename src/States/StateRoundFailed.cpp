#include <States\StateRoundFailed.h>
#include <Managers\StateManager.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\MessageHandler.h>
#include <iostream>
#include <Game\GameEvent.h>

StateRoundFailed::StateRoundFailed(StateManager & stateManager, StateType stateType)
	: StateBase(stateManager, stateType),
	m_nextRoundBeginTimer(3.0f, true)
{
	std::cout << "Press 'R' to retry.\n";
}

void StateRoundFailed::draw(sf::RenderWindow & window)
{
}

void StateRoundFailed::update(float deltaTime)
{
	m_nextRoundBeginTimer.update(deltaTime);
	if (!m_nextRoundBeginTimer.isExpired())
	{
		return;
	}

	m_nextRoundBeginTimer.deactivate();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::ReloadCurrentLevel);
		m_stateManager.switchToAndRemoveState(StateType::Game, StateBase::getType());
	}
}
