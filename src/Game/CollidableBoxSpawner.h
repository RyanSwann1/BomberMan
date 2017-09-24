#pragma once

#include <Entities\Direction.h>
#include <SFML\Graphics.hpp>
#include <Game\Timer.h>

class EntityManager;
class CollidableBoxSpawner
{
public:
	CollidableBoxSpawner(EntityManager& entityManager);
	~CollidableBoxSpawner();

	bool isActive() const;
	void update(float deltaTime);

private:
	const sf::IntRect m_endingSpawnBounds;
	EntityManager& m_entityManager;
	sf::IntRect m_spawnBounds;
	sf::Vector2i m_currentSpawnPosition;
	Direction m_spawnDirection;
	Timer m_spawnTimer;
	bool m_active;

	void handleSpawnDirection();
	void startSpawningCollidableBoxes();
	void reduceSpawnSize();
	void spawnCollidableBox();
	void init();
};