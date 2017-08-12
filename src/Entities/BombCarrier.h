#pragma once

#include <Entities\Character.h>

class BombCarrier : public Character
{
public:
	BombCarrier(const std::string& name, EntityTag tag, const sf::Vector2f& spawnPosition, EntityManager& entityManager, int ID, bool collidable);

	void update(float deltaTime) override;

protected:
	virtual void placeBomb();
	void increaseBombQuantity();

private:
	int m_maxBombQuantity;
	int m_currentBombQuantity;
	Timer m_bombSpawnTimer;

	void handleBombTimer(float deltaTime);

	sf::Vector2f getBombSpawnPosition() const;
};