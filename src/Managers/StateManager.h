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

		std::unique_ptr<StateBase> getState(StateType stateType) const;

	private:
		std::unordered_map<StateType, std::function<std::unique_ptr<StateBase>()>> m_stateFactory;

		template <class State>
		void registerState(StateManager* stateManager, StateType stateType)
		{
			assert(m_stateFactory.find(stateType) == m_stateFactory.cend());
			m_stateFactory.emplace(stateType, [stateManager, stateType] () -> std::unique_ptr<StateBase>
			{
				return std::make_unique<State>(*stateManager, stateType);
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
	void switchToAndRemoveState(StateType stateToSwitchTo, std::vector<StateType> statesToRemove);
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

private:
	const StateFactory m_stateFactory;
	std::deque<std::unique_ptr<StateBase>> m_states;
	//std::vector<std::unique_ptr<StateBase>> m_preExistingStateQueue;
	std::vector<StateType> m_stateQueue;
	std::vector<StateType> m_removals;
	//std::unique_ptr<StateType> m_stateToSwap;

	void handleQueue();
	void handleRemovals();
	void purgeStates();
	void addStateToQueue(StateType newStateType);
};