#include "EntityManager.h"
#include <Locators\EntityManagerLocator.h>

//EntityFactory
EntityManager::EntityFactory::EntityFactory(EntityManager * entityManager)
{
	registerEntity<Entity>("Player", entityManager);
}

std::unique_ptr<Entity> EntityManager::EntityFactory::getEntity(const std::string & entityName, const sf::Vector2f & entityPosition, int entityID) const
{
	auto iter = m_entityFactory.find(entityName);
	assert(iter != m_entityFactory.cend());
	return iter->second(entityName, entityPosition, entityID);
}

//EntityManager
EntityManager::EntityManager()
	: m_entityFactory(this),
	m_entities(),
	m_entityCount(0)
{
	EntityManagerLocator::provide(*this);
}

void EntityManager::addEntity(std::string && entityName, float xPosition, float yPosition)
{
	m_entityQueue.emplace_back(std::move(entityName), sf::Vector2f(xPosition, yPosition));
	++m_entityCount;
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
		entity.get()->update(deltaTime);
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
}