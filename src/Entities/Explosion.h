#pragma once

#include "Entity.h"

class Explosion : public Entity
{
public:
	Explosion(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);

	void update(float deltaTime) override;
};