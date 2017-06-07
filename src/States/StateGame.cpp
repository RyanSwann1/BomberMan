#include <States\StateGame.h>
#include <iostream>

StateGame::StateGame(StateType stateType)
	: StateBase(stateType),
	m_tileSheetManager(),
	m_entityManager(),
	m_levelManager()
{
	std::cout << "You have entered the game.\n";
}

StateGame::~StateGame()
{
}

void StateGame::update(float deltaTime)
{

}

void StateGame::draw(sf::RenderWindow& window)
{
	m_levelManager.draw(window);
}