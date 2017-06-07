#include "Game.h"
#include <Utilities\Utilities.h>
#include <assert.h>

Game::Game()
	: m_window("BomberMan", sf::Vector2i(336, 336)),
	m_textureManager(),
	m_gameState(std::make_unique<StateGame>(StateType::Game)),
	m_clock(),
	m_timeElasped()
{
	
}

const Window & Game::getWindow() const
{
	return m_window;
}

void Game::update()
{
	m_gameState->update(m_timeElasped.asSeconds());
	m_window.update();
}

void Game::draw()
{
	m_gameState->draw(m_window.getWindow());
	m_window.display();
}

void Game::lateUpdate()
{
	m_timeElasped = m_clock.restart();
}