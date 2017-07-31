#pragma once

#include <States\StateBase.h>
#include <vector>
#include <deque>
#include <unordered_map>
#include <functional>
#include <assert.h>
#include <memory>

class StateManager
{
	class StateFactory
	{
	public:
		StateFactory(StateManager* stateManager);

		StateBase* getState(StateType stateType) const;

	private:
		std::unordered_map<StateType, std::function<StateBase*()>> m_stateFactory;

		template <class State>
		void registerState(StateManager* stateManager, StateType stateType)
		{
			assert(m_stateFactory.find(stateType) == m_stateFactory.cend());
			m_stateFactory.emplace(stateType, [stateManager, stateType] () -> StateBase*
			{
				return new State(*stateManager, stateType);
			});
		}
	};

public:
	StateManager();
	~StateManager();
	StateManager(const StateManager&) = delete;
	StateManager& operator=(const StateManager&) = delete;
	StateManager(StateManager&&) = delete;
	StateManager&& operator=(StateManager&&) = delete;

	StateType getCurrentStateType() const;

	void createState(StateType stateToCreate);
	void removeState(StateType stateToRemove);
	void switchToState(StateType stateToSwitch);
	void switchToAndRemoveState(StateType stateToSwitchTo, StateType stateToRemove);
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

private:
	const StateFactory m_stateFactory;
	std::deque<StateBase*> m_states;
	std::vector<StateType> m_stateQueue;
	std::vector<StateType> m_removals;
	std::unique_ptr<StateType> m_stateToSwap;

	void handleQueue();
	void handleRemovals();
	void handleSwapState();
	void purgeStates();
};