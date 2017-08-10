#include <Entities\BombCarrier.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Locators\AudioPlayerLocator.h>
#include <Audio\AudioPlayer.h>
#include <Managers\EntityManager.h>

BombCarrier::BombCarrier(const std::string & name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID, bool collidable)
	: Character(name, tag, spawnPosition, entityManager, ID, collidable),
	m_maxBombQuantity(1),
	m_currentBombQuantity(m_maxBombQuantity),
	m_bombSpawnTimer(2.0f, true)
{}

void BombCarrier::update(float deltaTime)
{
	Character::update(deltaTime);

	handleBombTimer(deltaTime);
}

void BombCarrier::placeBomb()
{
	if (!m_currentBombQuantity)
	{
		return;
	}

	AudioPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::PlaceBomb);
	--m_currentBombQuantity;
	m_entityManager.addEntity("Bomb", getBombSpawnPosition());
}

void BombCarrier::increaseBombQuantity()
{
	++m_currentBombQuantity;
}

void BombCarrier::handleBombTimer(float deltaTime)
{
	m_bombSpawnTimer.update(deltaTime);

	if (m_bombSpawnTimer.isExpired())
	{
		m_bombSpawnTimer.reset();
		m_currentBombQuantity = m_maxBombQuantity;
	}
}

sf::Vector2f BombCarrier::getBombSpawnPosition() const
{
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	const sf::Vector2f bombSpawnPosition(std::round(m_position.x / tileSize) * tileSize, std::round(m_position.y / tileSize) * tileSize);
	return bombSpawnPosition;
}