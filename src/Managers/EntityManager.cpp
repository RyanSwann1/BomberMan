#include "EntityManager.h"
#include <Entities\Player.h>
#include <Entities\Bomb.h>
#include <Entities\Explosion.h>
#include <Entities\Crate.h>
#include <Entities\Enemy.h>
#include <Locators\EntityManagerLocator.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <algorithm>

//EntityFactory
EntityManager::EntityFactory::EntityFactory(EntityManager * entityManager)
{
	registerEntity<Player>("Player", EntityTag::Player, entityManager);
	registerEntity<Bomb>("Bomb", EntityTag::Bomb, entityManager);
	registerEntity<Explosion>("Explosion", EntityTag::Explosion, entityManager);
	registerEntity<Enemy>("Enemy1", EntityTag::Enemy, entityManager);
	registerEntity<Enemy>("Enemy2", EntityTag::Enemy, entityManager);
	registerEntity<Enemy>("Enemy3", EntityTag::Enemy, entityManager);
	registerEntity<Crate>("Crate", EntityTag::Crate, entityManager);
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
	for (auto& entityInQueue : m_entityQueue)
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
	for (auto& entity : m_entities)
	{
		m_removals.push_back(entity->getID());
	}

	m_entityQueue.clear();
	m_entityCount = 0;
}

