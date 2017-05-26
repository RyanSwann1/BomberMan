#pragma once

#include <States\StateBase.h>

class StateGame : public StateBase
{
public:
	StateGame(StateType stateType);

	void update(float deltaTime) override;
	void draw(Window& window) override;
};