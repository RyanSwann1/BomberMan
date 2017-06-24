#pragma once

#include <Locators\StateManagerLocator.h>

class GameManager : private StateManagerLocator
{
public:
	GameManager();
	~GameManager();

private:
	int m_enemiesRemaining;

	void winGame();
	void decreaseEnemyCount();
	void resetEnemyCount();
};