#include "Bomb.h"
#include <Managers\EntityManager.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Game\GameLogic.h>
#include "CollisionHandler.h"
#include <Locators\AudioPlayerLocator.h>
#include <Audio\AudioPlayer.h>

Bomb::Bomb(const std::string& name, EntityTag tag, const sf::Vector2f & position, 
	EntityManager & entityManager, int entityID, bool collidable, BombPower bombPower, int explosionRadius)
	: Entity(name, tag, position, entityManager, entityID, collidable),
	m_bombPower(bombPower),
	m_explosionRadius(explosionRadius)
{}

BombPower Bomb::getPower() const
{
	return m_bombPower;
}

int Bomb::getExplosionRadius() const
{
	return m_explosionRadius;
}

void Bomb::update(float deltaTime)
{
	Entity::update(deltaTime);
	CollisionHandler::checkForEntityCollisions(m_position, m_entityManager, sf::Vector2f(), *this);

	if (m_animationPlayer.getCurrentAnimation(AnimationName::Default).getName() == AnimationName::Default 
		&& m_animationPlayer.getCurrentAnimation(AnimationName::Default).isFinished())
	{
		explode();
	}
}

void Bomb::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection)
{
	if (entity->getTag() == EntityTag::Explosion)
	{
 		explode();
	}
}

void Bomb::explode()
{	
	for (const auto& spawnPosition : getExplosionSpawnPositions())
	{
		m_entityManager.addEntity("Explosion", spawnPosition);
		AudioPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::BombExplode);
	}

	m_entityManager.removeEntity(Entity::getID());
}