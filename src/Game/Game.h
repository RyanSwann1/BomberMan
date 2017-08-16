#pragma once

#include <States\StateGame.h>
#include <Managers\TextureManager.h>
#include <Window.h>
#include <Audio\AudioPlayer.h>
#include <Managers\AudioClipManager.h>
#include <managers\FontManager.h>
#include <Game\MessageHandler.h>
#include <Game\GameEvent.h>
#include <Managers\StateManager.h>
#include <Locators\GameEventMessengerLocator.h>
#include <unordered_map>
#include <memory>

class Game
{
public:
	Game();

	const Window& getWindow() const;
	void update();
	void draw();
	void lateUpdate();

private:
	MessageHandler<GameEvent> m_gameEventMessenger;
	Window m_window;
	TextureManager m_textureManager;
	FontManager m_fontManager;
	AudioClipManager m_audioClipManager;
	AudioPlayer m_audioClipPlayer;
	StateManager m_stateManager;
	sf::Clock m_clock;
	sf::Time m_timeElasped;
};