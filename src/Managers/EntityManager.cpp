#include "EntityManager.h"
#include <Entities\Player.h>
#include <Entities\Bomb.h>
#include <Entities\Explosion.h>
#include <Entities\Crate.h>
#include <Entities\Enemy.h>
#include <Entities\PowerUp.h>
#include <Entities\BombLowPower.h>
#include <Entities\BombMediumPower.h>
#include <Entities\BombHighPower.h>
#include <Locators\EntityManagerLocator.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <algorithm>

//EntityFactory
EntityManager::EntityFactory::EntityFactory(EntityManager * entityManager)
{
	registerEntity<Player>("Player", EntityTag::Player, entityManager, false);
	//registerEntity<Bomb>("Bomb", EntityTag::Bomb, entityManager, false);
	registerEntity<Explosion>("Explosion", EntityTag::Explosion, entityManager, false);
	registerEntity<Enemy>("Enemy1", EntityTag::Enemy, entityManager, false);
	registerEntity<Enemy>("Enemy2", EntityTag::Enemy, entityManager, false);
	registerEntity<Enemy>("Enemy3", EntityTag::Enemy, entityManager, false);
	registerEntity<Crate>("Crate", EntityTag::Crate, entityManager, true);
	registerEntity<Entity>("CollidableTile", EntityTag::Solid, entityManager, true);
	registerEntity<PowerUp>("PowerUpSpeedBoost", EntityTag::PowerUp, entityManager, false);
	registerEntity<PowerUp>("PowerUpExtraBomb", EntityTag::PowerUp, entityManager, false);
	registerEntity<PowerUp>("PowerUpIncreaseBombPower", EntityTag::PowerUp, entityManager, false);
	registerEntity<BombLowPower>("BombLowPower", EntityTag::Bomb, entityManager, false);
	registerEntity<BombMediumPower>("BombMediumPower", EntityTag::Bomb, entityManager, false);
	registerEntity<BombHighPower>("BombHighPower", EntityTag::Bomb, entityManager, false);
}

std::unique_ptr<Entity> EntityManager::EntityFactory::getEntity(const std::string& name, const sf::Vector2f & entityPosition, int entityID) const
{
	auto iter = m_entityFactory.find(name);
	assert(iter != m_entityFactory.cend());
	return iter->second(name, entityPosition, entityID);
}

//EntityManager
EntityManager::EntityManager()
	: m_entityFactory(this),
	m_entities(),
	m_entityCount(0)
{
	EntityManagerLocator::provide(*this);
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&EntityManager::purgeEntities, this), "EntityManager", GameEvent::ClearMap);
}

EntityManager::~EntityManager()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("EntityManager", GameEvent::ClearMap);
}

const std::unique_ptr<Entity>& EntityManager::getEntity(int ID) const
{
	auto cIter = std::find_if(m_entities.cbegin(), m_entities.cend(), [ID](const auto& entity) { return entity->getID() == ID; });
	assert(cIter != m_entities.cend());
	return *cIter;
}

const std::vector<std::unique_ptr<Entity>>& EntityManager::getEntities() const
{
	return m_entities;
}

void EntityManager::addEntity(const std::string& name, const sf::Vector2f & position)
{
	m_entityQueue.emplace_back(position, name);
	++m_entityCount;
}

void EntityManager::removeEntity(int entityID)
{
	if (std::find(m_removals.cbegin(), m_removals.cend(), entityID) == m_removals.cend())
	{
		m_removals.push_back(entityID);
	}
}

void EntityManager::draw(sf::RenderWindow & window)
{
	for (auto& entity : m_entities)
	{
		entity.get()->draw(window);
	}
}

void EntityManager::update(float deltaTime)
{
	for (auto& entity : m_entities)
	{
		entity->update(deltaTime);
	}
	
	handleQueue();
	handleRemovals();
}

void EntityManager::handleQueue()
{
	for (const auto& entityInQueue : m_entityQueue)
	{
		m_entities.emplace_back(m_entityFactory.getEntity(entityInQueue.m_name, entityInQueue.m_position, m_entityCount));
		++m_entityCount;
	}

	m_entityQueue.clear();
}

void EntityManager::handleRemovals()
{
	for (auto iter = m_removals.begin(); iter != m_removals.end();)
	{
		removeActiveEntity(*iter);
		iter = m_removals.erase(iter);
	}
}

void EntityManager::removeActiveEntity(int entityID)
{
	auto iter = std::find_if(m_entities.begin(), m_entities.end(), [entityID](const auto& entity) {return entity.get()->getID() == entityID; });
	assert(iter != m_entities.cend());
	m_entities.erase(iter);
}

void EntityManager::purgeEntities()
{
	for (const auto& entity : m_entities)
	{
		m_removals.push_back(entity->getID());
	}

	m_entityQueue.clear();
	m_entityCount = 0;
}