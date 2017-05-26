#pragma once

#include <States\StateGame.h>
#include <Window.h>
#include <unordered_map>
#include <memory>

class Game
{
public:
	Game();
	~Game();

	const Window& getWindow() const;
	void update();
	void draw();
	void lateUpdate();

private:
	Window m_window;
	std::unique_ptr<StateBase> m_currentState;
	sf::Clock m_clock;
	sf::Time m_timeElasped;
};

