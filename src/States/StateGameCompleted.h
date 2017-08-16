#pragma once

#include "StateBase.h"

class StateGameCompleted : public StateBase
{
public:
	StateGameCompleted(StateManager& stateManager, StateType stateType);

private:
	void activateButton(GUIButtonName buttonName) override;
};