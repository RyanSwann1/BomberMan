#include <States\StateGame.h>
#include <iostream>

StateGame::StateGame(StateType stateType)
	: StateBase(stateType)
{
	std::cout << "You have entered the game.\n";
}

void StateGame::update(float deltaTime)
{

}

void StateGame::draw(sf::RenderWindow& window)
{
	m_levelManager.draw(window);
}