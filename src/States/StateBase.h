#pragma once

#include "StateType.h"
#include <GUI\GUI.h>
#include <Window.h>
enum class GUIButtonName;
class StateManager;
class StateBase
{
	friend class GUI;
public:
	StateBase(StateManager& stateManager, StateType stateType);

	virtual ~StateBase();

	StateType getType();

	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& window);

protected:
	StateManager& m_stateManager;
	GUI m_gui;

	virtual void activateButton(GUIButtonName) = 0;

private:
	const StateType m_stateType;
};