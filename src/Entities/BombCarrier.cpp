#include <Entities\BombCarrier.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Locators\AudioPlayerLocator.h>
#include <Audio\AudioPlayer.h>
#include <Managers\EntityManager.h>

BombCarrier::BombCarrier(const std::string & name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID, bool collidable)
	: Character(name, tag, spawnPosition, entityManager, ID, collidable),
	m_maxBombQuantity(5),
	m_totalBombQuantity(1),
	m_currentBombQuantity(m_totalBombQuantity),
	m_bombQuantityResetTimer(3.0f, true),
	m_bombSpawnTimer(0.5f, false),
	m_bombPower(BombPower::Low)
{}

void BombCarrier::update(float deltaTime)
{
	Character::update(deltaTime);

	handleBombSpawnTimer(deltaTime);
	handleBombQuantityResetTimer(deltaTime);
}

void BombCarrier::placeBomb()
{
	if (!m_currentBombQuantity)
	{
		return;
	}

	if (m_bombSpawnTimer.isActive() && !m_bombSpawnTimer.isExpired())
	{
		return;
	}

	AudioPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::PlaceBomb);
	--m_currentBombQuantity;
	m_bombSpawnTimer.activate();
	switch (m_bombPower)
	{
	case BombPower::Low :
	{
		m_entityManager.addEntity("BombLowPower", getBombSpawnPosition());
		break;
	}
	case BombPower::Medium :
	{
		m_entityManager.addEntity("BombMediumPower", getBombSpawnPosition());
		break;
	}
	case BombPower::High :
	{
		m_entityManager.addEntity("BombHighPower", getBombSpawnPosition());
		break;
	}
	}
}

void BombCarrier::increaseBombQuantity()
{
	if (m_totalBombQuantity < m_maxBombQuantity)
	{
		++m_totalBombQuantity;
		++m_currentBombQuantity;
	}
}

void BombCarrier::increaseBombPower()
{
	switch (m_bombPower)
	{
	case BombPower::Low :
	{
		m_bombPower = BombPower::Medium;
		break;
	}
	case BombPower::Medium :
	{
		m_bombPower = BombPower::High;
		break;
	}
	}
}

bool BombCarrier::isMoreBombs() const
{
	return (m_currentBombQuantity ? true : false);
}

void BombCarrier::handleBombSpawnTimer(float deltaTime)
{
	m_bombSpawnTimer.update(deltaTime);

	if (m_bombSpawnTimer.isExpired())
	{
		m_bombSpawnTimer.reset();
		m_bombSpawnTimer.deactivate();
	}
}

void BombCarrier::handleBombQuantityResetTimer(float deltaTime)
{
	m_bombQuantityResetTimer.update(deltaTime);

	if (m_bombQuantityResetTimer.isExpired())
	{
		m_bombQuantityResetTimer.reset();
		m_currentBombQuantity = m_totalBombQuantity;
	}
}

sf::Vector2f BombCarrier::getBombSpawnPosition() const
{
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	const sf::Vector2f bombSpawnPosition(std::round(m_position.x / tileSize) * tileSize, std::round(m_position.y / tileSize) * tileSize);
	return bombSpawnPosition;
}