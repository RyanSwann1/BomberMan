#include <Entities\BombCarrier.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Locators\AudioPlayerLocator.h>
#include <Audio\AudioPlayer.h>
#include <Managers\EntityManager.h>
#include <iostream>

BombCarrier::BombCarrier(const std::string & name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID, bool collidable)
	: Character(name, tag, spawnPosition, entityManager, ID, collidable),
	m_maxBombQuantity(5),
	m_totalBombQuantity(1),
	m_currentBombQuantity(m_totalBombQuantity),
	m_bombQuantityResetTimer(3.0f, true),
	m_bombSpawnTimer(0.5f, false)
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
	std::cout << "Spawn Bomb\n";
	m_entityManager.addEntity("Bomb", getBombSpawnPosition());
}

void BombCarrier::increaseBombQuantity()
{
	if (m_totalBombQuantity < m_maxBombQuantity)
	{
		++m_totalBombQuantity;
		++m_currentBombQuantity;
	}
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