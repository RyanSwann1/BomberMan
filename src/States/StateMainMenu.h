#pragma once

#include <States\StateBase.h>

class StateMainMenu : public StateBase
{
public:
	StateMainMenu(StateManager& stateManager, StateType type);

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

private:

};