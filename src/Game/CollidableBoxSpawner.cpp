#include <Game\CollidableBoxSpawner.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Managers\EntityManager.h>
#include <Game\MessageHandler.h>
#include <Managers\LevelManager.h>
#include <Locators\LevelManagerLocator.h>
#include <Locators\CollidableBoxSpawnerLocator.h>
#include <Entities\CollisionHandler.h>
#include <Game\GameLogic.h>

CollidableBoxSpawner::CollidableBoxSpawner(EntityManager& entityManager)
	: m_endingSpawnBounds(7, 7, 13, 13),
	m_entityManager(entityManager),
	m_spawnBounds(4, 4, 16, 16),
	m_spawnDirection(Direction::Right),
	m_spawnTimer(.25f, false),
	m_active(false),
	m_currentSpawnPosition(m_spawnBounds.left, m_spawnBounds.top)
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&CollidableBoxSpawner::startSpawningCollidableBoxes, this), "CollidableBoxSpawner", GameEvent::SpawnCollidableBoxes);
	gameEventMessenger.subscribe(std::bind(&CollidableBoxSpawner::init, this), "CollidableBoxSpawner", GameEvent::ReloadCurrentLevel);
	gameEventMessenger.subscribe(std::bind(&CollidableBoxSpawner::init, this), "CollidableBoxSpawner", GameEvent::ClearMap);
	CollidableBoxSpawnerLocator::provide(*this);
}

CollidableBoxSpawner::~CollidableBoxSpawner()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("CollidableBoxSpawner", GameEvent::SpawnCollidableBoxes);
	gameEventMessenger.unsubscribe("CollidableBoxSpawner", GameEvent::ReloadCurrentLevel);
	gameEventMessenger.unsubscribe("CollidableBoxSpawner", GameEvent::ClearMap);
}

bool CollidableBoxSpawner::isActive() const
{
	return m_active;
}

void CollidableBoxSpawner::update(float deltaTime)
{
	m_spawnTimer.update(deltaTime);
	if (!m_active)
	{
		return;
	}
	if (!m_spawnTimer.isExpired())
	{
		return;
	}
	
	spawnCollidableBox();
	handleSpawnDirection();
	m_spawnTimer.reset();
}

void CollidableBoxSpawner::handleSpawnDirection()
{
	switch (m_spawnDirection)
	{
	case Direction::Right:
	{
		if (m_currentSpawnPosition.x < m_spawnBounds.width)
		{
			++m_currentSpawnPosition.x;
		}
		else
		{
			m_spawnDirection = Direction::Down;
		}
		break;
	}
	case Direction::Down:
	{
		if (m_currentSpawnPosition.y < m_spawnBounds.height)
		{
			++m_currentSpawnPosition.y;
		}
		else
		{
			m_spawnDirection = Direction::Left;
		}
		break;
	}
	case Direction::Left:
	{
		if (m_currentSpawnPosition.x > m_spawnBounds.left)
		{
			--m_currentSpawnPosition.x;
		}
		else
		{
			m_spawnDirection = Direction::Up;
		}
		break;
	}
	case Direction::Up:
	{
		if (m_currentSpawnPosition.y > m_spawnBounds.top)
		{
			--m_currentSpawnPosition.y;
		}
		else
		{
			reduceSpawnSize();
			m_spawnDirection = Direction::Right;
		}
		break;
	}
	}
}

void CollidableBoxSpawner::startSpawningCollidableBoxes()
{
	m_active = true;
	m_spawnTimer.activate();
}

void CollidableBoxSpawner::reduceSpawnSize()
{
	++m_spawnBounds.top;
	++m_spawnBounds.left;
	--m_spawnBounds.height;
	--m_spawnBounds.width;
	m_currentSpawnPosition.x = m_spawnBounds.left;
	m_currentSpawnPosition.y = m_spawnBounds.top;

	if (m_spawnBounds == m_endingSpawnBounds)
	{
		m_active = false;
		m_spawnTimer.reset();
		m_spawnTimer.deactivate();
	}
}

void CollidableBoxSpawner::spawnCollidableBox()
{
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	GameLogic::removeEntityAtPosition(sf::Vector2f(m_currentSpawnPosition.x * tileSize, m_currentSpawnPosition.y * tileSize), m_entityManager);	
	const sf::Vector2f spawnPosition(m_currentSpawnPosition.x * tileSize, m_currentSpawnPosition.y * tileSize);
	m_entityManager.addEntity("CollidableTile", spawnPosition);
}

void CollidableBoxSpawner::init()
{
	m_spawnBounds = sf::IntRect(4, 4, 16, 16);
	m_spawnDirection = Direction::Right;
	m_active = false;
	m_currentSpawnPosition = sf::Vector2i(m_spawnBounds.left, m_spawnBounds.top);
	m_spawnTimer.reset();
	m_spawnTimer.deactivate();
}