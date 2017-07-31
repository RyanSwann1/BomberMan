#pragma once

#include <Locators\StateManagerLocator.h>
#include <SFML\Graphics.hpp>
#include <Game\Timer.h>

enum class Direction;
class GameManager : private StateManagerLocator
{
public:
	GameManager();
	~GameManager();

	void update(float deltaTime);

private:
	const float m_totalGameTime;
	int m_enemiesRemaining;
	Timer m_gameTimer;
	bool m_reduceMapSize;

	void winGame();
	void onEnemyDeath();
	void resetEnemyCount();
	void onPlayerDeath();
	void reduceMapSize();
	
};