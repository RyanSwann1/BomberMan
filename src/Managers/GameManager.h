#pragma once

#include <Locators\StateManagerLocator.h>
#include <Managers\EntityManager.h>
#include <SFML\Graphics.hpp>
#include <Game\Timer.h>

enum class GameEvent;
enum class Direction;
class GameManager : private StateManagerLocator
{
	class FunctionInQueue
	{
	public:

	};

public:
	GameManager(EntityManager& entityManager);
	~GameManager();

	void increaseEntityBombExplosiveRadius(const std::unique_ptr<Entity>& entity, int entityID);
	void update(float deltaTime);


private:
	const int m_maxEnemies;
	const float m_totalGameTime;
	EntityManager& m_entityManager;
	int m_enemiesRemaining;
	Timer m_gameTimer;
	Timer m_nextRoundTimer;
	std::unordered_map<GameEvent, std::function<void()>> m_functionQueue;

	Timer m_spawnTimer;
	bool m_reduceMapSize;
	sf::Vector2i m_currentSpawnPosition;
	Direction m_spawnDirection;

	void onWinGame();
	void onEnemyDeath();
	void onPlayerDeath();
	void reduceMapSize();
	void assignEnemyToAggressive();
	void onEnemySpawn();
	void onLevelReload();
};