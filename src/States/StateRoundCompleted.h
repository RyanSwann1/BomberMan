#pragma once

#include "StateBase.h"

class StateRoundCompleted : public StateBase
{
public:
	StateRoundCompleted(StateManager& stateManager, StateType stateType);
	~StateRoundCompleted();

private:
	void activateButton(GUIButtonName buttonName) override;
	void onWinGame();
};