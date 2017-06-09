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
	m_entityManager.update(deltaTime);
}

void StateGame::draw(sf::RenderWindow& window)
{
	m_levelManager.draw(window);
	m_entityManager.draw(window);
}