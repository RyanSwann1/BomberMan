#pragma once

#include <States\StateGame.h>
#include <Managers\TextureManager.h>
#include <Window.h>
#include <Audio\AudioClipPlayer.h>
#include <Managers\AudioClipManager.h>
#include <Game\MessageHandler.h>
#include <Game\GameEvent.h>
#include <Managers\StateManager.h>
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
	Window m_window;
	StateManager m_stateManager;
	TextureManager m_textureManager;
	AudioClipManager m_audioClipManager;
	AudioClipPlayer m_audioClipPlayer;
	MessageHandler<GameEvent> m_gameEventMessenger;
	sf::Clock m_clock;
	sf::Time m_timeElasped;
};

