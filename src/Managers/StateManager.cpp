#include "StateManager.h"
#include <States\StateGame.h>
#include <States\StateGameCompleted.h>
#include <States\StateRoundCompleted.h>
#include <States\StateRoundFailed.h>
#include <States\StateMainMenu.h>
#include <States\StatePauseMenu.h>
#include <Locators\StateManagerLocator.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\MessageHandler.h>
#include <Managers\StateManager.h>
#include <iostream>

//StateFactory
StateManager::StateFactory::StateFactory(StateManager* stateManager)
{
	registerState<StateGame>(stateManager, StateType::Game);
	registerState<StateGameCompleted>(stateManager, StateType::GameCompleted);
	registerState<StateRoundCompleted>(stateManager, StateType::RoundCompleted);
	registerState<StateRoundFailed>(stateManager, StateType::RoundFailed);
	registerState<StateMainMenu>(stateManager, StateType::MainMenu);
	registerState<StatePauseMenu>(stateManager, StateType::PauseMenu);
}

std::unique_ptr<StateBase> StateManager::StateFactory::getState(StateType stateType) const
{
	auto iter = m_stateFactory.find(stateType);
	assert(iter != m_stateFactory.cend());
	return iter->second();
}

//StateManager
StateManager::StateManager()
	: m_stateFactory(this),
	m_states(),
	m_stateQueue(),
	m_removals()
{
	StateManagerLocator::provide(*this);
}

StateManager::~StateManager()
{
	purgeStates();
}

void StateManager::removeState(StateType stateToRemove)
{
	if (std::find_if(m_removals.cbegin(), m_removals.cend(), [stateToRemove]
	(const auto& stateType) { return stateType == stateToRemove; }) != m_removals.cend())
	{
		return;
	}

	m_removals.push_back(stateToRemove);
}

void StateManager::purgeStates()
{
	m_states.clear();
}

void StateManager::addStateToQueue(StateType newStateType)
{
	assert(std::find_if(m_stateQueue.cbegin(), m_stateQueue.cend(), 
		[newStateType](const auto stateType) { return newStateType == stateType; }) == m_stateQueue.cend());
	
	m_stateQueue.push_back(newStateType);
}

StateType StateManager::getCurrentStateType() const
{
	assert(!m_states.empty());
	return m_states.back()->getType();
}

void StateManager::switchToState(StateType stateToSwitch)
{
	if (m_states.empty())
	{
		m_states.emplace_back(m_stateFactory.getState(stateToSwitch));
		return;
	}

	addStateToQueue(stateToSwitch);
}

void StateManager::switchToAndRemoveState(StateType stateToSwitchTo, StateType stateToRemove)
{
	switchToState(stateToSwitchTo);
	removeState(stateToRemove);
}

void StateManager::switchToAndRemoveState(StateType stateToSwitchTo, std::vector<StateType> statesToRemove)
{
	switchToState(stateToSwitchTo);
	for (const auto stateToRemove : statesToRemove)
	{
		removeState(stateToRemove);
	}
}

void StateManager::update(float deltaTime)
{
	for (auto& state : m_states)
	{
		state->update(deltaTime);
	}

	handleQueue();
	handleRemovals();
}

void StateManager::draw(sf::RenderWindow & window)
{
	for (auto& state : m_states)
	{
		state->draw(window);
	}
}

void StateManager::handleQueue()
{
	while (!m_stateQueue.empty())
	{
		for (auto iter = m_stateQueue.begin(); iter != m_stateQueue.end();)
		{
			bool stateAlreadyExists = false;
			for (auto& state : m_states)
			{
				if (state->getType() != *iter)
				{
					continue;
				}

				state.swap(m_states.back());
				stateAlreadyExists = true;
				iter = m_stateQueue.erase(iter);
				if (m_stateQueue.empty())
				{
					return;
				}
			}

			if (!stateAlreadyExists)
			{
				m_states.push_back(m_stateFactory.getState(*iter));
				iter = m_stateQueue.erase(iter);
				continue;
			}
		}
	}
}

void StateManager::handleRemovals()
{
	for (const auto stateToRemove : m_removals)
	{
		auto state = std::find_if(m_states.begin(), m_states.end(), [stateToRemove](const auto& state) { return state->getType() == stateToRemove; });
		assert(state != m_states.cend());
		m_states.erase(state);
	}

	m_removals.clear();
}

void StateManager::createState(StateType stateToCreate)
{
	auto newState = m_stateFactory.getState(stateToCreate);
	m_states.push_front(std::move(newState));
}