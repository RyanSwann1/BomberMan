#pragma once

#include "Entity.h"

class Explosion : public Entity
{
public:
	Explosion(const std::string& name, EntityTag tag, const sf::Vector2f& spawnPosition, EntityManager& entityManager, int ID, bool collidable);

	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;
	void update(float deltaTime) override;
};