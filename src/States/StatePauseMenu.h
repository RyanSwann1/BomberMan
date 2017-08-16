#pragma once

#include <States\StateBase.h>

class StatePauseMenu : public StateBase
{
public:
	StatePauseMenu(StateManager& stateManager, StateType type);

private:
	void activateButton(GUIButtonName buttonName) override;
};