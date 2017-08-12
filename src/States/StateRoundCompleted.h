#pragma once

#include "StateBase.h"

class StateRoundCompleted : public StateBase
{
public:
	StateRoundCompleted(StateManager& stateManager, StateType stateType);

	void draw(sf::RenderWindow& window) override;
	void update(float deltaTime) override;
};