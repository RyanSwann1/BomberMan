#include <Entities\PowerUp.h>
#include <Managers\EntityManager.h>

PowerUp::PowerUp(const std::string & name, EntityTag tag, const sf::Vector2f & spawnPosition, EntityManager & entityManager, int ID, bool collidable)
	: Entity(name, tag, spawnPosition, entityManager, ID, collidable)
{
}

void PowerUp::update(float deltaTime)
{
	
}

void PowerUp::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect & intersection)
{
	switch (entity->getTag())
	{
	case EntityTag::Explosion:
	{
		m_entityManager.removeEntity(Entity::getID());
		break;
	}
	case EntityTag::Enemy:
	{
		m_entityManager.removeEntity(Entity::getID());
		break;
	}
	case EntityTag::Player:
	{
		m_entityManager.removeEntity(Entity::getID());
		break;
	}
	}
}
