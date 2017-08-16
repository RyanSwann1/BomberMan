#pragma once

#include <States\StateBase.h>
#include <Game\Timer.h>

class StateRoundFailed : public StateBase
{
public:
	StateRoundFailed(StateManager& stateManager, StateType stateType);

private:
	void activateButton(GUIButtonName buttonName) override;
};