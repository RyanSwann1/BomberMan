#include "Enemy.h"
#include <Managers\EntityManager.h>
#include <Locators\AudioPlayerLocator.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Locators\LevelManagerLocator.h>
#include <managers\LevelManager.h>
#include <Level\Level.h>
#include <Game\MessageHandler.h>
#include <Audio\AudioPlayer.h>
#include <Game\GameLogic.h>
#include "CollisionHandler.h"

Enemy::Enemy(const std::string & name, const sf::Vector2f & position, EntityManager & entityManager, int entityID)
	: Character(name, position, entityManager, entityID),
	m_visitedLocations(),
	m_locationID(0),
	m_frontier(),
	m_movementTimer(1)
{}

void Enemy::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection)
{
	Character::handleEntityCollision(entity, intersection);
	if (entity->getName() == "Explosion")
	{
		if(m_lives <= 0)
		{
			AudioPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::EnemyDeath);
			GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::EnemyDeath);
		}
	}
}

void Enemy::draw(sf::RenderWindow & window)
{
	Entity::draw(window);

	for (const auto& rect : m_graph)
	{
		window.draw(rect);
	}
}

void Enemy::update(float deltaTime)
{
	if (!m_visitedLocations.empty())
	{
		Entity::update(deltaTime);
		move(deltaTime);
		return;
	}

	handleAlgorithm();
	Character::update(deltaTime);
}

std::vector<Enemy::Location> Enemy::getNeighbourLocations(const sf::Vector2i & position, const EntityManager & entityManager, const std::unique_ptr<Level>& currentLevel, const std::vector<VisitedLocation>& visitedLocations)
{
	std::vector<Enemy::Location> moveLocations;
	//Search x axis
	for (int x = position.x - 1; x <= position.x + 1; ++x)
	{
		//if (x == position.x)
		//{
		//	continue;
		//}

		auto iter = std::find_if(visitedLocations.cbegin(), visitedLocations.cend(), [x, position]
		(const auto& visitedLocation) {return visitedLocation.m_location == sf::Vector2i(x, position.y); });
		if (iter != visitedLocations.cend())
		{
			continue;
		}

		if (CollisionHandler::isTileAtPosition(sf::Vector2i(x, position.y)))
		{
			continue;
		}

		const sf::Vector2i location(position);
		auto iiter = std::find_if(m_visitedLocations.cbegin(), m_visitedLocations.cend(),
			[&location](const auto& visitedLocation) {return visitedLocation.m_location == location; });
		assert(iiter != m_visitedLocations.cend());

		//Come from position
		moveLocations.emplace_back(sf::Vector2i(x, position.y), iiter->m_ID);
	}

	//Search y axis
	for (int y = position.y - 1; y <= position.y + 1; ++y)
	{
		//if (y == position.y)
		//{
		//	continue;
		//}

		auto iter = std::find_if(visitedLocations.cbegin(), visitedLocations.cend(), [position, y]
		(const auto& visitedLocation) {return visitedLocation.m_location == sf::Vector2i(position.x, y); });
		if (iter != visitedLocations.cend())
		{
			continue;
		}
		
		if (CollisionHandler::isTileAtPosition(sf::Vector2i(position.x, y)))
		{
			continue;
		}

		const sf::Vector2i location(position);
		auto iiter = std::find_if(m_visitedLocations.cbegin(), m_visitedLocations.cend(),
			[&location](const auto& visitedLocation) {return visitedLocation.m_location == location; });
		assert(iiter != m_visitedLocations.cend());

		//Come from position
		moveLocations.emplace_back(sf::Vector2i(position.x, y), iiter->m_ID);
	}

	return moveLocations;
}

std::unique_ptr<Entity>* Enemy::scanSurroundingArea(const sf::Vector2f & position, const EntityManager & entityManager, const std::unique_ptr<Level>& currentLevel)
{
	return nullptr;
}

void Enemy::handleAlgorithm()
{
	const auto& currentLevel = LevelManagerLocator::getLevelManager().getCurrentLevel();
	const int tileSize = currentLevel->getTileSize();
	const sf::Vector2i initialPosition(std::floor(m_position.x / tileSize), std::floor(m_position.y / tileSize));
	m_frontier.emplace_back(initialPosition, 0);
	m_visitedLocations.emplace_back(initialPosition, m_locationID, 0);

	//http://www.redblobgames.com/pathfinding/a-star/introduction.html
	//http://www.redblobgames.com/pathfinding/a-star/implementation.html#cplusplus
	while (!m_frontier.empty())
	{
		const Location& location = m_frontier.front();
		const auto& pos = location.m_location;
		auto iter = std::find_if(m_visitedLocations.cbegin(), m_visitedLocations.cend(),
			[&pos](const auto& iter) {return iter.m_location == pos; });
		if (iter == m_visitedLocations.cend())
		{
			//sf::RectangleShape rect;
			//rect.setPosition(sf::Vector2f(location.x * 16 + 4, location.y * 16 + 4));
			//rect.setSize(sf::Vector2f(8, 8));
			//rect.setFillColor(sf::Color::Red);
			//m_graph.push_back(rect);

			++m_locationID;
			m_visitedLocations.emplace_back(location.m_location, m_locationID, location.m_cameFromID);
		}

		const auto neighbourLocations = getNeighbourLocations(location.m_location, m_entityManager, currentLevel, m_visitedLocations);
		for (const auto& neighbourLocation : neighbourLocations)
		{
			m_frontier.emplace_back(neighbourLocation.m_location, neighbourLocation.m_cameFromID);
		}


		m_frontier.pop_front();
	}
}

void Enemy::move(float deltaTime)
{
	//MESSY
	//Pointers only for collision tile validation
	//Will change
	const auto& currentLevel = LevelManagerLocator::getLevelManager().getCurrentLevel();
	const auto* playerLocation = getEntityLocation(currentLevel, m_entityManager, "Player");
	const auto* enemyLocation = getEntityLocation(currentLevel, m_entityManager, Entity::getName());
	int moveToID = 0; 
	if (playerLocation)
	{ 
		moveToID = playerLocation->m_cameFromID;
	}
	if (playerLocation && enemyLocation)
	{
		
		while (true)
		{
			const auto& location = getLocation(moveToID);
			if (location.m_cameFromID > enemyLocation->m_ID)
			{
				moveToID = location.m_cameFromID;
				continue;
			}
			break;
		}
	}

	m_movementTimer.update(deltaTime);
	if (m_movementTimer.isExpired())
	{
		if (playerLocation && enemyLocation)
		{
			const auto& moveToLocation = getLocation(moveToID).m_location;
			m_position = sf::Vector2f(moveToLocation.x * 16, moveToLocation.y * 16);
		}
	
		m_movementTimer.reset();
	}
}

const Enemy::VisitedLocation* Enemy::getEntityLocation(const std::unique_ptr<Level>& currentLevel, const EntityManager & entityManager, const std::string & entityName)
{
	const int tileSize = currentLevel->getTileSize();
	const auto& entityPosition = GameLogic::getEntityPosition(entityManager, entityName);
	if (CollisionHandler::isTileAtPosition(sf::Vector2i(std::floor(entityPosition.x / tileSize), std::floor(entityPosition.y / tileSize))))
	{
		return nullptr;
	}
	const sf::Vector2i position(std::floor(GameLogic::getEntityPosition(entityManager, entityName).x / tileSize),
		std::floor(GameLogic::getEntityPosition(entityManager, entityName).y / tileSize));


	auto iter = std::find_if(m_visitedLocations.cbegin(), m_visitedLocations.cend(),
		[&position](const auto& visitedLocation) { return visitedLocation.m_location == position; });
	assert(iter != m_visitedLocations.cend());
	return &(*iter);
}

const Enemy::VisitedLocation & Enemy::getLocation(int locationID) const
{
	auto iter = std::find_if(m_visitedLocations.cbegin(), m_visitedLocations.cend(), 
		[locationID](const auto& location) { return location.m_ID == locationID; });
	assert(iter != m_visitedLocations.cend());
	return *iter;
}