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

	class Graph
	{
	public:
		Graph(const sf::Vector2f& entityCentrePosition, const EntityManager& entityManager, int tileSize);

		bool isPointSafeDistanceFromPoint(const sf::Vector2f& entityCentrePosition, const Point& requestedPoint, int tileSize) const;
		bool isPointInRadiusOfHarm(const sf::Vector2f& point, int tileSize) const;
		bool isTargetNeighbouringTargetPoint(const sf::Vector2f& targetPoint, EntityTag entityTag, int tileSize) const;
		bool isTargetAtTargetPoint(const sf::Vector2f& targetPoint, State currentState, int tileSize) const;
		bool isPointOnGraph(const std::vector<Point>& graph, const sf::Vector2f& point) const;
		std::vector<sf::Vector2f> getNeighbouringPointsFromPoint(const sf::Vector2f& startingPoint, int tileSize) const;
		std::vector<sf::Vector2f> getNeighbouringPointsOnEntity(const sf::Vector2f& startingPoint, int tileSize, EntityTag entityTag) const;
		std::vector<sf::Vector2f> getNeighbouringPointsOnGraphContainingBomb(const sf::Vector2f& startingPoint, int tileSize) const;
		const Point* getPointOnGraph(const sf::Vector2f& point) const;
		const Point* getPointOnGraph(int ID) const;
		const Point& getPointAtCurrentPosition(const sf::Vector2f& entityCentrePosition, int tileSize) const;

		const std::vector<Point>& getGraph() const;
		bool isPowerUpFound() const;
		bool isOpponentFound() const;
		const sf::Vector2f& getPowerUpAtPoint() const;
		const sf::Vector2f& getOpponentAtPoint() const;

	private:
		const EntityManager& m_entityManager;
		std::vector<Point> m_graph;
 		bool m_powerUpFound;
		bool m_opponentFound;
		sf::Vector2f m_powerUpAtPoint;
		sf::Vector2f m_opponentAtPoint;
	
		void createGraph(const sf::Vector2f& entityCentrePosition, int tileSize);
		void addNeighbouringPointsToFrontier(const Point& point, std::deque<Point>& frontier, int& pointID, int tileSize);
		void addNewPoint(const sf::Vector2f & position, std::deque<Point>& frontier, int& pointID, int cameFromID);
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
	bool m_correctPosition;
	sf::Vector2f m_walkingPoint;


	void move(const Graph& graph, float deltaTime, int tileSize);
	void handleMovement(const Graph& graph, float deltaTime, int tileSize);
	void assignWalkingPoint(const Graph& graph, const std::deque<sf::Vector2f>& pointsToMoveTo, int tileSize);
	void handleNewMoveDirection(const Graph& graph, int tileSize);

	bool isPointSafeFromBombsAtPoint(const sf::Vector2f& point, int tileSize) const;
	bool neighbouringCrateAtPoint(const std::vector<sf::Vector2f>& points, int tileSize) const;
	

	void setStateToTargetPlayer();
	void handleStates(const Graph& graph, int tileSize);
	void updateLocalHazardousPoints(const Graph& graph, int tileSize);
	void assignNewMovementDirection(const sf::Vector2f& nextPointToMoveTo, int tileSize);
	void setTargetPointAtOpponent(const Graph& graph, int tileSize);

	void setTargetPointBesideCrate(const Graph& graph, int tileSize);
	void assignPointsToMoveTo(const Graph& graph, std::deque<sf::Vector2f>& pointsToMoveTo, int tileSize);
	void setNewTargetPoint(const sf::Vector2f& point);
	bool reachedTargetPoint(const Graph& graph, int tileSize) const;
	void setTargetPointAtSafePoint(const Graph& graph, int tileSize);
	void setState(State newState);
	void setTypeToAggressive(EntityMessage& entityMessage);
	void setTargetPointAtPowerUp(const sf::Vector2f& powerUpPoint);
};