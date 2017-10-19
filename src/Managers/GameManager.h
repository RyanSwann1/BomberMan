#pragma once

#include <Locators\StateManagerLocator.h>
#include <Managers\EntityManager.h>
#include <SFML\Graphics.hpp>
#include <Game\Timer.h>

class GameManager : private StateManagerLocator
{
public:
	GameManager();
	~GameManager();

	const Timer& getGameTimer() const;

	void update(float deltaTime);

private:
	const float m_startCollidableBoxesTime;
	const int m_maxEnemies;
	int m_enemiesRemaining;
	Timer m_gameTimer;

	void onWinGame();
	void onEnemyDeath();
	void onPlayerDeath();
	void onEnemySpawn();
	void onLevelReload();
	void handleGameTimer();
};