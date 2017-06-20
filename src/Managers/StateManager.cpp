#include "StateManager.h"
#include <States\StateGame.h>
#include <States\StateWinGame.h>
#include <Locators\StateManagerLocator.h>
#include <algorithm>

//StateFactory
StateManager::StateFactory::StateFactory()
{
	registerState<StateGame>(StateType::Game);
	registerState<StateWinGame>(StateType::WinGame);
}

std::unique_ptr<StateBase> StateManager::StateFactory::getState(StateType stateType) const
{
	auto iter = m_stateFactory.find(stateType);
	assert(iter != m_stateFactory.cend());
	return iter->second();
}

StateManager::StateManager()
	: m_stateFactory(),
	m_states(),
	m_stateQueue(),
	m_removals(),
	m_currentState(nullptr)
{
	StateManagerLocator::provide(*this);
}

void StateManager::removeState(StateType stateType)
{
	m_removals.push_back(stateType);
}

void StateManager::switchToState(StateType stateType)
{
	if (m_states.empty())
	{
		createState(stateType);
		return;
	}

	m_stateQueue.push_back(stateType);
}

void StateManager::update(float deltaTime)
{
	assert(m_currentState);
	m_currentState->update(deltaTime);

	handleQueue();
	handleRemovals();
}

void StateManager::draw(sf::RenderWindow & window)
{
	assert(m_currentState);
	m_currentState->draw(window);
}

void StateManager::handleQueue()
{
	for (auto iter = m_stateQueue.begin(); iter != m_stateQueue.end();)
	{
		createState(*iter);
		iter = m_stateQueue.erase(iter);
	}
}

void StateManager::handleRemovals()
{
	for (auto iter = m_removals.begin(); iter != m_removals.end();)
	{
		auto state = std::find_if(m_states.begin(), m_states.end(), [iter](const auto& state) {return state.get()->getType() == *iter; });
		assert(state != m_states.cend());
		if (m_currentState && m_currentState->getType() == *iter)
		{
			m_currentState = nullptr;
		}
		m_states.erase(state);
		iter = m_removals.erase(iter);
	}
}

void StateManager::createState(StateType stateType)
{
	m_states.emplace_back(m_stateFactory.getState(stateType));
	m_currentState = m_states.back().get();
}