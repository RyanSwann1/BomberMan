#pragma once

#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);

	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;
};