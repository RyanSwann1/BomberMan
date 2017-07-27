#pragma once

#include <States\StateBase.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <assert.h>

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
			m_stateFactory.emplace(stateType, [stateManager, stateType]() -> std::unique_ptr<StateBase>
			{
				return std::make_unique<State>(*stateManager, stateType);
			});
		}
	};

public:
	StateManager();
	StateManager(const StateManager&) = delete;
	StateManager& operator=(const StateManager&) = delete;
	StateManager(StateManager&&) = delete;
	StateManager&& operator=(StateManager&&) = delete;

	void switchToState(StateType stateToSwitch);
	void switchToAndRemoveState(StateType stateToSwitchTo, StateType stateToRemove);
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

private:
	StateFactory m_stateFactory;
	std::vector<std::unique_ptr<StateBase>> m_states;
	std::vector<StateType> m_stateQueue;
	std::vector<StateType> m_removals;
	std::unique_ptr<StateBase> m_tempStateToAdd;

	void handleQueue();
	void handleRemovals();
	void createState(StateType stateToCreate);
	void removeState(StateType stateToRemove);
};