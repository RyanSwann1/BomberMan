#pragma once

#include <Entities\Entity.h>
class PowerUp : public Entity
{
public:
	PowerUp(const std::string& name, EntityTag tag, const sf::Vector2f& spawnPosition, EntityManager& entityManager, int ID, bool collidable);

	void update(float deltaTime) override;
	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;
};