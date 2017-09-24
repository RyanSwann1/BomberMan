#pragma once

#include "StateBase.h"

class StateRoundCompleted : public StateBase
{
public:
	StateRoundCompleted(StateManager& stateManager, StateType stateType);
	~StateRoundCompleted();

private:
	void activateButton(const std::string& name) override;
	void onWinGame();
	void onPlayerDeath();
};