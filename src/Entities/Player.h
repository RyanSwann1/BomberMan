#pragma once

#include "Entity.h"

class Player : public Entity
{
	//https://www.youtube.com/watch?v=mjUrVGy2hA8&t=903s
public:
	Player(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);

	void update(float deltaTime) override;

private:
	
	void placeBomb();
	void handleAnimation() override;
};