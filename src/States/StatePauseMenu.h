#pragma once

#include <States\StateBase.h>

class StatePauseMenu : public StateBase
{
public:
	StatePauseMenu(StateManager& stateManager, StateType type);

	void update(float deltaTime) override {}
	void draw(sf::RenderWindow& window) override {}
};