#include "Game.h"
#include <assert.h>

Game::Game()
	: m_window("SFML_WINDOW", sf::Vector2f(480, 600)),
	m_currentState(std::make_unique<StateGame>(StateType::Game)),
	m_clock(),
	m_timeElasped()
{
}

Game::~Game()
{
}

const Window & Game::getWindow() const
{
	return m_window;
}

void Game::update()
{
	assert(m_currentState.get());
	m_currentState->update(m_timeElasped.asSeconds());
	m_window.update();
}

void Game::draw()
{
	assert(m_currentState.get());
	m_currentState->draw(m_window);
	m_window.display();
}

void Game::lateUpdate()
{
	m_timeElasped = m_clock.restart();
}
