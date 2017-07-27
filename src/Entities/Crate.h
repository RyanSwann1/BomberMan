#pragma once

#include "Entity.h"

class Crate : public Entity
{
public:
	Crate(const std::string& name, EntityTag tag, const sf::Vector2f& spawnPosition, EntityManager& entityManager, int ID);

	void update(float deltaTime) override;
	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;
};