#pragma once

#include <Entities\Bomb.h>
class BombMediumPower : public Bomb
{
public:
	BombMediumPower(const std::string& name, EntityTag tag, const sf::Vector2f& position, EntityManager& entityManager, int entityID, bool collidable);

private:
	std::vector<sf::Vector2f> getExplosionSpawnPositions() const override;
};