#pragma once

#include <Entities\Bomb.h>
class BombLowPower : public Bomb
{
public:
	BombLowPower(const std::string& name, EntityTag tag, const sf::Vector2f& position, EntityManager& entityManager, int entityID, bool collidable);

private:
	std::vector<sf::Vector2f> getExplosionSpawnPositions() const override;
};