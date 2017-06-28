#pragma once

#include "Character.h"
#include <Game\Timer.h>
#include <deque>

class Level;
class Enemy : public Character
{
	class Location
	{
	public:
		Location(const sf::Vector2i& location, int cameFromID)
			: m_location(location),
			m_cameFromID(cameFromID)
		{}


		const sf::Vector2i m_location;
		const int m_cameFromID;
	};

	class VisitedLocation
	{
	public:
		VisitedLocation(const sf::Vector2i& position, int ID, int cameFromID)
			: m_location(position),
			m_ID(ID),
			m_cameFromID(cameFromID)
		{}

		const sf::Vector2i m_location;
		const int m_ID;
		const int m_cameFromID;
	};

public:
	Enemy(const std::string& name, const sf::Vector2f& position, EntityManager& entityManager, int entityID);

	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;
	void draw(sf::RenderWindow& window) override;
	void update(float deltaTime) override;

private:
	std::vector<VisitedLocation> m_visitedLocations;

	std::vector<sf::RectangleShape> m_graph;

	int m_locationID;
	Timer m_movementTimer;

	std::deque<Location> m_frontier;

	std::vector<Location> getNeighbourLocations(const sf::Vector2i& position,
		const EntityManager& entityManager, const std::unique_ptr<Level>& currentLevel, const std::vector<VisitedLocation>& visitedLocations);

	std::unique_ptr<Entity>* scanSurroundingArea(const sf::Vector2f& position,
		const EntityManager& entityManager, const std::unique_ptr<Level>& currentLevel);

	
	void handleAlgorithm();
	void move(float deltaTime);
	const VisitedLocation* getEntityLocation(const std::unique_ptr<Level>& currentLevel, const EntityManager& entityManager, const std::string& entityName);
	const VisitedLocation& getLocation(int locationID) const;
};