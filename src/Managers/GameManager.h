#pragma once

#include <Locators\StateManagerLocator.h>
#include <Managers\EntityManager.h>
#include <SFML\Graphics.hpp>
#include <Game\Timer.h>

enum class GameEvent;
enum class Direction;
class GameManager : private StateManagerLocator
{
public:
	GameManager(EntityManager& entityManager);
	~GameManager();

	void update(float deltaTime);

private:
	const int m_maxEnemies;
	const float m_totalGameTime;
	EntityManager& m_entityManager;
	int m_enemiesRemaining;
	Timer m_gameTimer;
	
	Timer m_spawnTimer;
	bool m_reduceMapSize;
	sf::Vector2i m_currentSpawnPosition;
	Direction m_spawnDirection;

	void winGame();
	void onEnemyDeath();
	void onPlayerDeath();
	void reduceMapSize();
	void assignEnemyToAggressive();
	void onEnemySpawn();
};