#pragma once

#include <States\StateGame.h>
#include <Managers\TextureManager.h>
#include <Window.h>
#include <Audio\AudioClipPlayer.h>
#include <Managers\AudioClipManager.h>
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
	TextureManager m_textureManager;
	AudioClipManager m_audioClipManager;
	AudioClipPlayer m_audioClipPlayer;
	std::unique_ptr<StateBase> m_gameState;
	sf::Clock m_clock;
	sf::Time m_timeElasped;
};

