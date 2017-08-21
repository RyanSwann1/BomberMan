#pragma once

#include "BombCarrier.h"
#include <Game\Timer.h>
#include <deque>
#include <memory>

enum class EnemyType
{
	Passive = 0,
	Aggressive,
	Total
};

class EntityMessage;
class Enemy : public BombCarrier
{
	enum class State
	{
		TargettingOpponent = 0,
		TargettingCrates,
		TargettingPowerUp,
		SetTargetPointAtOpponent,
		SetTargetPointAtCrate,
		SetTargetPointAtSafePoint,
		SetTargetPointAtPowerUp,
		InitializeState,
		MovingToSafePoint,
		StopMovement,
		SetNewBombLocation
	};

	class Point
	{
	public:
		Point(const sf::Vector2i& position, int ID, int cameFromID)
			: m_point(position),
			m_ID(ID),
			m_cameFromID(cameFromID)
		{}

		const sf::Vector2i m_point;
		const int m_ID;
		const int m_cameFromID;
	};

public:
	Enemy(const std::string& name, EntityTag tag, const sf::Vector2f& position, EntityManager& entityManager, int entityID, bool collidable);
	~Enemy();

	EnemyType getType() const { return m_type; }

	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;
	void update(float deltaTime) override;

private:
	Timer m_movementTimer;
	Timer m_stopMovementTimer;
	sf::Vector2i m_targetPoint;
	bool m_reachedTargetPoint;
	std::vector<sf::Vector2i> m_bombAtPoints;
	State m_state;
	EnemyType m_type;

	void setStateToTargetPlayer();
	void checkExistingBombsAtPoints(int tileSize);
	void move(const std::vector<Point>& graph, const Point& point, int tileSize);
	void handleStates(const std::vector<Point>& graph, const sf::Vector2i& powerUpAtPoint, const sf::Vector2i& opponentAtPoint, bool opponentFound, bool powerUpFound, int tileSize);
	void handleBombAtPoints(const std::vector<Point>& graph, int tileSize);

	void initializeGraph(sf::Vector2i& powerUpAtPoint, sf::Vector2i& opponentAtPoint, std::vector<Point>& graph, bool& opponentFound, bool& powerUpFound, int tileSize);
	void addNeighbouringPointsToFrontier(sf::Vector2i& powerUpAtPoint, sf::Vector2i& opponentAtPoint, const Point& point, std::vector<Point>& graph, std::deque<Point>& frontier, 
		int& pointID, bool& opponentFound, bool& powerUpFound, int tileSize);	
	void addNewPoint(const sf::Vector2i& position, std::vector<Point>& graph, std::deque<Point>& frontier, int& pointID, int cameFromID);
	void setTargetPointAtCrate(const std::vector<Point>& graph, int tileSize);
	void moveToTargetPoint(const std::vector<Point>& graph, int tileSize);
	void setNewTargetPoint(const sf::Vector2i& point);
	bool reachedTargetPoint(const std::vector<Point>& graph, int tileSize) const;
	bool isTargetNeighbouringTargetPoint(const std::vector<Point>& graph, EntityTag entityTag, int tileSize) const;
	bool isTargetAtTargetPoint(const std::vector<Point>& graph, EntityTag entityTag, int tileSize) const;
	bool isPointOnGraph(const std::vector<Point>& graph, const sf::Vector2i& point) const;

	std::vector<sf::Vector2i> getNeighbouringPointsOnGraph(const sf::Vector2i& startingPoint, const std::vector<Point>& graph, int tileSize) const;
	std::vector<sf::Vector2i> getNeighbouringPointsOnEntity(const sf::Vector2i& startingPoint, int tileSize, EntityTag entityTag) const;
	std::vector<sf::Vector2i> getNeighbouringPointsOnGraphContainingBomb(const sf::Vector2i& startingPoint, const std::vector<Point>& graph, int tileSize) const;
	const Point& getPointOnGraph(const std::vector<Point>& graph, const sf::Vector2i& position) const;
	const Point& getPointOnGraph(const std::vector<Point>& graph, int ID) const;
	bool isPointInRadiusOfHarm(const std::vector<Point>& graph, const Point& point, int tileSize) const;

	void setTargetPointAtSafePoint(const std::vector<Point>& graph, int tileSize);
	bool neighbouringCrateAtPoint(const std::vector<sf::Vector2i>& points, int tileSize) const;
	bool isPointAppropriateDistanceAway(const std::vector<Point>& graph, const Point& requestedPoint, int tileSize) const;
	bool isPointSafeFromBombsAtPoint(const std::vector<Point>& graph, const sf::Vector2i& point, int tileSize) const;
	const Point& getPointAtPosition(const std::vector<Point>& graph, int tileSize) const;	
	void setState(State newState);
	void setTypeToAggressive(EntityMessage& entityMessage);
};