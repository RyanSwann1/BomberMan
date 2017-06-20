#pragma once

#include "StateType.h"
#include <Window.h>

class StateBase
{
public:
	StateBase(StateType stateType)
		: m_stateType(stateType)
	{

	}
	virtual ~StateBase()
	{

	}

	StateType getType() const { return m_stateType; }

	virtual void update(float deltaTime) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

private:
	const StateType m_stateType;
};