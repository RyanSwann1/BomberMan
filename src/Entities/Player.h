#pragma once

#include "BombCarrier.h"

class Player : public BombCarrier
{
public:
	Player(const std::string& name, EntityTag tag, const sf::Vector2f& spawnPosition, EntityManager& entityManager, int ID, bool collidable);
	~Player();

	void update(float deltaTime) override;
	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;
	void handlePowerUpCollision(const std::unique_ptr<Entity>& entity);
};