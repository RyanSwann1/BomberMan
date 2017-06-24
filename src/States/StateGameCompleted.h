#pragma once

#include "StateBase.h"

class StateGameCompleted : public StateBase
{
public:
	StateGameCompleted(StateManager& stateManager, StateType stateType);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

private:

};