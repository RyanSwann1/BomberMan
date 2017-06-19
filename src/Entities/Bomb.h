#pragma once

#include "Entity.h"

class Bomb : public Entity
{
public:
	Bomb(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);
	
	void update(float deltaTime) override;
	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;

private:
	void explode();
};