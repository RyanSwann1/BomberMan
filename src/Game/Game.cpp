#include "Game.h"
#include <Utilities\Utilities.h>
#include "DebugOverlay.h"
#include <Locators\GameEventMessengerLocator.h>
#include <assert.h>

Game::Game()
	: m_gameEventMessenger(),
	m_window(m_gameEventMessenger, "BomberMan", sf::Vector2i(336, 336)),
	m_textureManager(),
	m_stateManager(),
	m_fontManager(),
	m_guiManager(m_gameEventMessenger, m_fontManager),
	m_audioClipManager(),
	m_audioClipPlayer(),
	m_clock(),
	m_timeElasped()
{
	GameEventMessengerLocator::provide(m_gameEventMessenger);
	m_stateManager.switchToState(StateType::MainMenu);
}

const Window & Game::getWindow() const
{
	return m_window;
}

void Game::update()
{
	m_stateManager.update(m_timeElasped.asSeconds());
	m_audioClipPlayer.update();
	m_guiManager.update(m_window.getRenderWindow());
	m_window.update();
}

void Game::draw()
{
	m_stateManager.draw(m_window.getRenderWindow());
	m_guiManager.draw(m_window.getRenderWindow());
	DebugOverlay::draw(m_window.getRenderWindow());
	m_window.display();
}

void Game::lateUpdate()
{
	m_timeElasped = m_clock.restart();
}