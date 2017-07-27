#include "StateManager.h"
#include <States\StateGame.h>
#include <States\StateGameCompleted.h>
#include <States\StateRoundCompleted.h>
#include <States\StateRoundFailed.h>
#include <Locators\StateManagerLocator.h>
#include <algorithm>

//StateFactory
StateManager::StateFactory::StateFactory(StateManager* stateManager)
{
	registerState<StateGame>(stateManager, StateType::Game);
	registerState<StateGameCompleted>(stateManager, StateType::GameCompleted);
	registerState<StateRoundCompleted>(stateManager, StateType::RoundCompleted);
	registerState<StateRoundFailed>(stateManager, StateType::RoundFailed);
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
	m_removals(),
	m_tempStateToAdd()
{
	StateManagerLocator::provide(*this);
}

void StateManager::removeState(StateType stateToRemove)
{
	m_removals.push_back(stateToRemove);
}

void StateManager::switchToState(StateType stateToSwitch)
{
	if (m_states.empty())
	{
		createState(stateToSwitch);
		return;
	}


	for (auto iter = m_states.begin(); iter != m_states.end(); ++iter)
	{
		if (iter->get()->getType() == stateToSwitch)
		{
			m_tempStateToAdd = std::move(*iter);
			return;
		}
	}
	
	m_stateQueue.push_back(stateToSwitch);
}

void StateManager::switchToAndRemoveState(StateType stateToSwitchTo, StateType stateToRemove)
{
	switchToState(stateToSwitchTo);
	removeState(stateToRemove);
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
	for (auto iter = m_stateQueue.begin(); iter != m_stateQueue.end();)
	{
		createState(*iter);
		iter = m_stateQueue.erase(iter);
	}

	if (m_tempStateToAdd)
	{
		m_states.emplace_back(std::move(m_tempStateToAdd));
		m_tempStateToAdd.release();
	}
}

void StateManager::handleRemovals()
{
	for (auto iter = m_states.begin(); iter != m_states.end();)
	{
		if (!*iter)
		{
			iter = m_states.erase(iter);
			continue;
		}

		++iter;
	}

	for (auto iter = m_removals.begin(); iter != m_removals.end();)
	{
		auto state = std::find_if(m_states.begin(), m_states.end(), [iter](const auto& state) {return state.get()->getType() == *iter; });
		assert(state != m_states.cend());

		m_states.erase(state);
		iter = m_removals.erase(iter);
	}
}

void StateManager::createState(StateType stateToCreate)
{
	auto& newState = m_stateFactory.getState(stateToCreate);
	m_states.emplace_back(std::move(newState));
}