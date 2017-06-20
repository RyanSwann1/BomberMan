#include "StateWinGame.h"
#include <iostream>

StateWinGame::StateWinGame(StateType stateType)
	: StateBase(stateType)
{
	std::cout << "You have won the game.";
}

void StateWinGame::update(float deltaTime)
{

}

void StateWinGame::draw(sf::RenderWindow & window)
{
}
