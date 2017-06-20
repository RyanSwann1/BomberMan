#pragma once

class GameManager
{
public:
	GameManager();
	~GameManager();

private:
	int m_enemiesRemaining;

	void winGame();
	void loseGame();
	void decreaseEnemyCount();
	void resetEnemyCount();
};