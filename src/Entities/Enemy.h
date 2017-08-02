#pragma once

#include "Character.h"
#include <Game\Timer.h>
#include <deque>
#include <memory>

class Enemy : public Character
{
	enum class Type
	{
		Passive = 0,
		Aggressive,
		Total
	};

	enum class State
	{
		TargetOpponent = 0,
		TargettingCrates,
		setTargetPointAtCrate,
		setTargetPointAtSafePoint,
		InitializeState,
		MovingToSafePoint,
		StopMovement
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

	void handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection) override;
	void update(float deltaTime) override;

private:

	//afronym : my advice would be to change m_graph to a 2d array of all tiles
	//afronym : you'd be storing the blocked tiles, but it should make some things easier
	//afronym : finding the opponent's & enemy's tiles based on position would be super easy
	//afronym : and visualizing it(for debugging) will be super easy

	Timer m_movementTimer;
	Timer m_stopMovementTimer;
	Timer m_bombScannerTimer;
	sf::Vector2i m_targetPoint;
	bool m_reachedTargetPoint;
	std::vector<sf::Vector2i> m_bombAtPoints;
	State m_state;
	Type m_type;

	void setStateToTargetPlayer();
	void checkExistingBombsAtPoints(int tileSize);
	void move(const std::vector<Point>& graph, const Point& point, int tileSize);
	void handleStates(const std::vector<Point>& graph, const sf::Vector2i& opponentAtPoint, bool opponentFound, int tileSize);
	void handleBombAtPoints(const std::vector<Point>& graph, int tileSize);

	void initializeGraph(sf::Vector2i& opponentAtPoint, std::vector<Point>& graph, bool& opponentFound, int tileSize);
	void Enemy::addNeighbouringPointsToFrontier(sf::Vector2i& opponentAtPoint, const Point& point, std::vector<Point>& graph, std::deque<Point>& frontier, 
		int& pointID, bool& opponentFound, int tileSize);	
	void addNewPoint(const sf::Vector2i& position, std::vector<Point>& graph, std::deque<Point>& frontier, int& pointID, int cameFromID);
	void setTargetPointAtCrate(const std::vector<Point>& graph, int tileSize);
	void moveToTargetPoint(const std::vector<Point>& graph, int tileSize);
	void setNewTargetPoint(const sf::Vector2i& point);
	bool reachedTargetPoint(const std::vector<Point>& graph, int tileSize) const;
	bool isTargetNeighbouringTargetPoint(const std::vector<Point>& graph, EntityTag entityTag, int tileSize) const;
	bool isPointOnGraph(const std::vector<Point>& graph, const sf::Vector2i& point) const;

	std::vector<sf::Vector2i> getNeighbouringPointsOnGraph(const sf::Vector2i& startingPoint, const std::vector<Point>& graph, int tileSize) const;
	std::vector<sf::Vector2i> getNeighbouringPointsOnCrates(const sf::Vector2i& startingPoint, int tileSize) const;
	std::vector<sf::Vector2i> getNeighbouringPointsOnGraphContainingBomb(const sf::Vector2i& startingPoint, const std::vector<Point>& graph, int tileSize) const;
	const Point& getPointOnGraph(const std::vector<Point>& graph, const sf::Vector2i& position) const;
	const Point& getPointOnGraph(const std::vector<Point>& graph, int ID) const;
	const sf::Vector2i getNeighbouringPointOnGraph(const sf::Vector2i& point, const std::vector<Point>& graph, int tileSize) const;
	bool isPointInRadiusOfHarm(const std::vector<Point>& graph, const Point& point, int tileSize) const;

	void setTargetPointAtSafePoint(const std::vector<Point>& graph, int tileSize);
	bool neighbouringCrateAtPoint(const std::vector<sf::Vector2i>& points, int tileSize) const;
	bool isPointAppropriateDistanceAway(const std::vector<Point>& graph, const Point& requestedPoint, int tileSize) const;
	bool isPointSafeFromBombsAtPoint(const std::vector<Point>& graph, const sf::Vector2i& point, int tileSize) const;
	const Point& getCurrentPoint(const std::vector<Point>& graph, int tileSize) const;	
	void setState(State newState);
};