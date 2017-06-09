#pragma once

#include <Entities\Entity.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <SFML\Graphics.hpp>
#include <assert.h>

class EntityManager
{
	class EntityInQueue
	{
	public:
		EntityInQueue(std::string&& entityName, const sf::Vector2f& position)
			: m_name(std::move(entityName)),
			m_position(position)
		{

		}

		const std::string m_name;
		const sf::Vector2f m_position;
	};

	class EntityFactory
	{
	public:
		EntityFactory(EntityManager* entityManager);
		std::unique_ptr<Entity> getEntity(const std::string& entityName, const sf::Vector2f& entityPosition, int entityID) const;

	private:
		std::unordered_map<std::string, std::function<std::unique_ptr<Entity>(const std::string&, const sf::Vector2f&, int)>> m_entityFactory;
	
		template<class EntityType>
		void registerEntity(std::string&& entityName, EntityManager* entityManager)
		{
			assert(m_entityFactory.find(entityName) == m_entityFactory.cend());
			m_entityFactory.emplace(std::move(entityName), [entityManager](const std::string& name, 
				const sf::Vector2f& position, int entityID) -> std::unique_ptr<Entity>
			{
				return std::make_unique<EntityType>(name, position, *entityManager, entityID);
			});
		}
	};

public:
	EntityManager();

	void addEntity(std::string&& entityName, float xPosition, float yPosition);
	void draw(sf::RenderWindow& window);
	void update(float deltaTime);

private:
	EntityFactory m_entityFactory;
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::vector<EntityInQueue> m_entityQueue;
	int m_entityCount;

	void handleQueue();
	void handleRemovals();
};