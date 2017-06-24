#include "StateGameCompleted.h"
#include <iostream>

StateGameCompleted::StateGameCompleted(StateManager& stateManager, StateType stateType)
	: StateBase(stateManager, stateType)
{
	std::cout << "You have won the game.";
}

void StateGameCompleted::update(float deltaTime)
{

}

void StateGameCompleted::draw(sf::RenderWindow & window)
{
}
