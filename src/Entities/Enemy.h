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
		TargettingCrate,
		TargettingPowerUp,
		SetTargetPointAtOpponent,
		SetTargetPointBesideCrate,
		SetTargetPointAtSafePoint,
		SetTargetPointAtPowerUp,
		AssignNewState,
		MovingToSafePoint,
		StopMovement,
	};

	class Point
	{
	public:
		Point(const sf::Vector2f& position, int ID, int cameFromID)
			: m_point(position),
			m_ID(ID),
			m_cameFromID(cameFromID)
		{}

		const sf::Vector2f m_point;
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
	Timer m_stopMovementTimer;
	Timer m_moveErrorTimer;
	sf::Vector2f m_targetPoint;
	bool m_reachedTargetPoint;
	bool m_chooseNewMoveDirection;
	State m_state;
	EnemyType m_type;
	std::vector<sf::Vector2f> m_localHazardousPoints;
	std::deque<sf::Vector2f> m_walkingPoints;

	//Movement - Temporary
	Direction m_moveDirection;
	bool m_correctPosition;
	sf::Vector2f m_walkingPoint;
	void move(const std::vector<Point>& graph, float deltaTime, int tileSize);
	void handleMovement(const std::vector<Point>& graph, float deltaTime, int tileSize);
	//Walking Point
	void assignWalkingPoint(const std::vector<Point>& graph, const std::deque<sf::Vector2f>& pointsToMoveTo, int tileSize);
	void handleNewMoveDirection(const std::vector<Point>& graph, int tileSize);
	void handleExistingHazardousPoints(int tileSize);

	void setStateToTargetPlayer();
	void handleStates(const std::vector<Point>& graph, const sf::Vector2f& powerUpAtPoint, const sf::Vector2f& opponentAtPoint, bool opponentFound, bool powerUpFound, int tileSize);
	void updateLocalHazardousPoints(const std::vector<Point>& graph, int tileSize);
	void assignNewMovementDirection(const sf::Vector2f& nextPointToMoveTo, int tileSize);
	void setTargetPointAtOpponent(const std::vector<Point>& graph, const sf::Vector2f& opponentAtPoint, int tileSize);

	void initializeGraph(sf::Vector2f& powerUpAtPoint, sf::Vector2f& opponentAtPoint, std::vector<Point>& graph, bool& opponentFound, bool& powerUpFound, int tileSize);
	void addNeighbouringPointsToFrontier(sf::Vector2f& powerUpAtPoint, sf::Vector2f& opponentAtPoint, const Point& point, std::vector<Point>& graph, std::deque<Point>& frontier,
		int& pointID, bool& opponentFound, bool& powerUpFound, int tileSize);
	void addNewPoint(const sf::Vector2f& position, std::vector<Point>& graph, std::deque<Point>& frontier, int& pointID, int cameFromID);
	void setTargetPointBesideCrate(const std::vector<Point>& graph, int tileSize);
	void assignPointsToMoveTo(const std::vector<Point>& graph, std::deque<sf::Vector2f>& pointsToMoveTo, int tileSize);
	void setNewTargetPoint(const sf::Vector2f& point);
	bool reachedTargetPoint(const std::vector<Point>& graph, int tileSize) const;
	bool isTargetNeighbouringTargetPoint(const std::vector<Point>& graph, EntityTag entityTag, int tileSize) const;
	bool isPointOnGraph(const std::vector<Point>& graph, const sf::Vector2f& point) const;

	std::vector<sf::Vector2f> getNeighbouringPointsFromPoint(const sf::Vector2f& startingPoint, const std::vector<Point>& graph, int tileSize) const;
	std::vector<sf::Vector2f> getNeighbouringPointsOnEntity(const sf::Vector2f& startingPoint, int tileSize, EntityTag entityTag) const;
	std::vector<sf::Vector2f> getNeighbouringPointsOnGraphContainingBomb(const sf::Vector2f& startingPoint, const std::vector<Point>& graph, int tileSize) const;
	const Point* getPointOnGraph(const std::vector<Point>& graph, const sf::Vector2f& point) const;
	const Point* getPointOnGraph(const std::vector<Point>& graph, int ID) const;
	bool isPointInRadiusOfHarm(const std::vector<Point>& graph, const sf::Vector2f& point, int tileSize) const;
	bool isTargetAtTargetPoint(const std::vector<Point>& graph, int tileSize) const;

	void setTargetPointAtSafePoint(const std::vector<Point>& graph, int tileSize);
	bool neighbouringCrateAtPoint(const std::vector<sf::Vector2f>& points, int tileSize) const;
	bool isPointAppropriateDistanceAway(const std::vector<Point>& graph, const Point& requestedPoint, int tileSize) const;
	bool isPointSafeFromBombsAtPoint(const sf::Vector2f& point, int tileSize) const;
	const Point& getPointAtCurrentPosition(const std::vector<Point>& graph, int tileSize) const;
	void setState(State newState);
	void setTypeToAggressive(EntityMessage& entityMessage);
	void setTargetPointAtPowerUp(const sf::Vector2f& powerUpPoint);
};