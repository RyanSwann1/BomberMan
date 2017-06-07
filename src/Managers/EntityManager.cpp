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
	return iter->second(entityPosition, entityID);
}

//EntityManager
EntityManager::EntityManager()
	: m_entityFactory(this),
	m_entities(),
	m_entityCount(0)
{
	EntityManagerLocator::provide(*this);
	
}

void EntityManager::addEntity(float xPosition, float yPosition)
{
	m_entities.emplace_back(std::make_unique<Entity>(sf::Vector2f(xPosition, yPosition), *this, m_entityCount));
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
}