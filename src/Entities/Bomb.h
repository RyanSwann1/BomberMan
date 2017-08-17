#pragma once

#include "Entity.h"
#include <Entities\BombPower.h>

class Bomb : public Entity
{
public:
	Bomb(const std::string& name, EntityTag tag, const sf::Vector2f& position, 
		EntityManager& entityManager, int entityID, bool collidable, BombPower power, int explosionRadius);
	
	BombPower getPower() const;
	int getExplosionRadius() const;

	void update(float deltaTime) override;
	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;

protected:
	virtual std::vector<sf::Vector2f> getExplosionSpawnPositions() const = 0;
	const BombPower m_bombPower;
	const int m_explosionRadius;

private:
	void explode();
};