#pragma once

#include <Entities\Character.h>
#include <Entities\BombPower.h>

class BombCarrier : public Character
{
public:
	BombCarrier(const std::string& name, EntityTag tag, const sf::Vector2f& spawnPosition, EntityManager& entityManager, int ID, bool collidable);

	void update(float deltaTime) override;

protected:
	virtual void placeBomb();
	void increaseBombQuantity();
	void increaseBombPower();

private:
	const int m_maxBombQuantity;
	int m_totalBombQuantity;
	int m_currentBombQuantity;
	Timer m_bombQuantityResetTimer;
	Timer m_bombSpawnTimer;
	BombPower m_bombPower;
	

	void handleBombSpawnTimer(float deltaTime);
	void handleBombQuantityResetTimer(float deltaTime);

	sf::Vector2f getBombSpawnPosition() const;
};