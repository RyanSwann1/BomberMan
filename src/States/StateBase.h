#pragma once

#include "StateType.h"
#include <Window.h>

class StateManager;
class StateBase
{
public:
	StateBase(StateManager& stateManager, StateType stateType)
		: m_stateType(stateType),
		m_stateManager(stateManager)
	{

	}
	virtual ~StateBase()
	{

	}

	StateType getType() const { return m_stateType; }

	virtual void update(float deltaTime) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

protected:
	StateManager& m_stateManager;

private:
	const StateType m_stateType;
};