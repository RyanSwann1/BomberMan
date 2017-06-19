#include "Game.h"
#include <Utilities\Utilities.h>
#include <Locators\GameEventMessengerLocator.h>
#include <assert.h>

Game::Game()
	: m_window("BomberMan", sf::Vector2i(336, 336)),
	m_textureManager(),
	m_audioClipManager(),
	m_audioClipPlayer(),
	m_gameEventMessenger(),
	m_gameState(),
	m_clock(),
	m_timeElasped()
{
	GameEventMessengerLocator::provide(m_gameEventMessenger);
	m_gameState = std::make_unique<StateGame>(StateType::Game);
}

const Window & Game::getWindow() const
{
	return m_window;
}

void Game::update()
{
	m_gameState->update(m_timeElasped.asSeconds());
	m_audioClipPlayer.update();
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