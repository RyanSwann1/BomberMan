#pragma once

#include <States\StateBase.h>

class StateMainMenu : public StateBase
{
public:
	StateMainMenu(StateManager& stateManager, StateType type);

private:
	void activateButton(const std::string& name) override;
};