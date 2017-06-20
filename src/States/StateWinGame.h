#pragma once

#include "StateBase.h"

class StateWinGame : public StateBase
{
public:
	StateWinGame(StateType stateType);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

private:

};