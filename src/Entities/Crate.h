#pragma once

#include "Entity.h"

class Crate : public Entity
{
public:
	Crate(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);

	void update(float deltaTime) override;
	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;
};