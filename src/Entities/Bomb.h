#pragma once

#include "Entity.h"

class Bomb : public Entity
{
public:
	Bomb(const std::string& name, EntityTag tag, const sf::Vector2f& position, EntityManager& entityManager, int entityID, bool collidable);
	
	void update(float deltaTime) override;
	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;

private:
	void explode();
	std::vector<sf::Vector2f> getExplosionSpawnPositions() const;
};