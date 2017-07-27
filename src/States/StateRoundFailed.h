#pragma once

#include <States\StateBase.h>
#include <Game\Timer.h>

class StateRoundFailed : public StateBase
{
public:
	StateRoundFailed(StateManager& stateManager, StateType stateType);

	void draw(sf::RenderWindow& window) override;
	void update(float deltaTime) override;

private:
	Timer m_nextRoundBeginTimer;
};