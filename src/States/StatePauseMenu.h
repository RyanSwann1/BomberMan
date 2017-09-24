#pragma once

#include <States\StateBase.h>

class StatePauseMenu : public StateBase
{
public:
	StatePauseMenu(StateManager& stateManager, StateType type);

private:
	void activateButton(const std::string& name) override;
};