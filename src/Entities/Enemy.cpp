#include "Enemy.h"
#include "CollisionHandler.h"
#include <Managers\EntityManager.h>
#include <Locators\AudioPlayerLocator.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>
#include <Level\Level.h>
#include <Game\MessageHandler.h>
#include <Audio\AudioPlayer.h>
#include <Game\GameLogic.h>
#include <Game\RandomNumberGenerator.h>
#include <Entities\EntityMessage.h>
#include <Locators\EntityMessengerLocator.h>
#include <Game\EntityMessenger.h>
#include <Game\DebugOverlay.h>
#include <random>
#include <iostream>

//Graph
Enemy::Graph::Graph(const sf::Vector2f& entityCentrePosition, const EntityManager& entityManager, int tileSize)
	: m_entityManager(entityManager),
	m_graph(),
	m_powerUpFound(false),
	m_opponentFound(false),
	m_powerUpAtPoint(),
	m_opponentAtPoint()
{
	createGraph(entityCentrePosition, tileSize);
}

bool Enemy::Graph::isPointSafeDistanceFromPoint(const sf::Vector2f& entityCentrePosition, const Point & requestedPoint, int tileSize) const
{
	const auto& pointAtPosition = getPointAtCurrentPosition(entityCentrePosition, tileSize);
	if (requestedPoint.m_point.x != pointAtPosition.m_point.x && requestedPoint.m_point.y != pointAtPosition.m_point.y)
	{
		return true;
	}

	int moveSpacesAway = 3;
	int nextPointID = requestedPoint.m_ID;
	while (nextPointID != pointAtPosition.m_ID)
	{
		const auto& nextPoint = m_graph[nextPointID];
		if (nextPoint.m_cameFromID == pointAtPosition.m_ID)
		{
			break;
		}
		nextPointID = nextPoint.m_cameFromID;
		--moveSpacesAway;
	}

	return (moveSpacesAway <= 0 ? true : false);
}

bool Enemy::Graph::isPointInRadiusOfHarm(const sf::Vector2f & point, int tileSize) const
{
	const int searchRadius = 5;
	//x
	for (int x = point.x - searchRadius; x != point.x + searchRadius; ++x)
	{
		if (!isPointOnGraph(m_graph, point))
		{
			continue;
		}

		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(x, point.y), m_entityManager, EntityTag::Bomb, tileSize))
		{
			return true;
		}
	}

	//y
	for (int y = point.y - searchRadius; y != point.y + searchRadius; ++y)
	{
		if (!isPointOnGraph(m_graph, point))
		{
			continue;
		}

		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(point.x, y), m_entityManager, EntityTag::Bomb, tileSize))
		{
			return true;
		}
	}

	return false;
}

bool Enemy::Graph::isTargetNeighbouringTargetPoint(const sf::Vector2f& targetPoint, EntityTag entityTag, int tileSize) const
{
	switch (entityTag)
	{
	case EntityTag::Crate:
	{
		return (getNeighbouringPointsOnEntity(targetPoint, tileSize, EntityTag::Crate).empty() ? false : true);
	}
	default:
		return false;
	}
}

bool Enemy::Graph::isTargetAtTargetPoint(const sf::Vector2f& targetPoint, State currentState, int tileSize) const
{
	bool targetAtTargetPoint = false;
	switch (currentState)
	{
	case State::TargettingCrate:
	{
		targetAtTargetPoint = isTargetNeighbouringTargetPoint(targetPoint, EntityTag::Crate, tileSize);
		break;
	}
	case State::TargettingPowerUp:
	{
		targetAtTargetPoint = CollisionHandler::isEntityAtPosition(sf::Vector2f(targetPoint.x, targetPoint.y),
			m_entityManager, EntityTag::PowerUp, tileSize);
		break;
	}
	}

	return targetAtTargetPoint;
}

bool Enemy::Graph::isPointOnGraph(const std::vector<Point>& graph, const sf::Vector2f & point) const
{
	auto cIter = std::find_if(m_graph.cbegin(), m_graph.cend(),
		[&point](const auto& pointOnGraph) { return pointOnGraph.m_point == point; });

	return (cIter != graph.cend() ? true : false);
}

std::vector<sf::Vector2f> Enemy::Graph::getNeighbouringPointsFromPoint(const sf::Vector2f & startingPoint, int tileSize) const
{
	std::vector<sf::Vector2f> neighbouringPoints;
	const sf::Vector2f searchSize(1, 1);
	const int incrementAmount = 2;
	//x
	for (int x = startingPoint.x - searchSize.x; x <= startingPoint.x + searchSize.x; x += incrementAmount)
	{
		if (!isPointOnGraph(m_graph, sf::Vector2f(x, startingPoint.y)))
		{
			continue;
		}

		neighbouringPoints.emplace_back(x, startingPoint.y);
	}
	//y
	for (int y = startingPoint.y - searchSize.y; y <= startingPoint.y + searchSize.y; y += incrementAmount)
	{
		if (!isPointOnGraph(m_graph, sf::Vector2f(startingPoint.x, y)))
		{
			continue;
		}

		neighbouringPoints.emplace_back(startingPoint.x, y);
	}

	return neighbouringPoints;
}

std::vector<sf::Vector2f> Enemy::Graph::getNeighbouringPointsOnEntity(const sf::Vector2f & startingPoint, int tileSize, EntityTag entityTag) const
{
	std::vector<sf::Vector2f> pointsWithCrates;
	//x
	for (int x = startingPoint.x - 1; x <= startingPoint.x + 1; x += 2)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(x, startingPoint.y), m_entityManager, entityTag, tileSize))
		{
			pointsWithCrates.emplace_back(x, startingPoint.y);
		}
	}

	//y
	for (int y = startingPoint.y - 1; y <= startingPoint.y + 1; y += 2)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(startingPoint.x, y), m_entityManager, entityTag, tileSize))
		{
			pointsWithCrates.emplace_back(startingPoint.x, y);
		}
	}

	return pointsWithCrates;
}

std::vector<sf::Vector2f> Enemy::Graph::getNeighbouringPointsOnGraphContainingBomb(const sf::Vector2f & startingPoint, int tileSize) const
{
	std::vector<sf::Vector2f> points;
	//x
	for (int x = startingPoint.x - 5; x <= startingPoint.x + 5; ++x)
	{
		if (!isPointOnGraph(m_graph, sf::Vector2f(x, startingPoint.y)))
		{
			continue;
		}

		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(x, startingPoint.y), m_entityManager, EntityTag::Bomb, tileSize))
		{
			points.emplace_back(x, startingPoint.y);
			continue;
		}
	}
	//y
	for (int y = startingPoint.y - 5; y <= startingPoint.y + 5; ++y)
	{
		if (!isPointOnGraph(m_graph, sf::Vector2f(startingPoint.x, y)))
		{
			continue;
		}

		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(startingPoint.x, y), m_entityManager, EntityTag::Bomb, tileSize))
		{
			points.emplace_back(startingPoint.x, y);
			continue;
		}
	}

	return points;
}

const Enemy::Point * Enemy::Graph::getPointOnGraph(const sf::Vector2f& position) const
{
	auto cIter = std::find_if(m_graph.cbegin(), m_graph.cend(),
		[&position](const auto& point) { return point.m_point == position; });

	return (cIter != m_graph.cend() ? &(*cIter) : nullptr);
}

const Enemy::Point * Enemy::Graph::getPointOnGraph(int ID) const
{
	return &m_graph[ID];
}

const Enemy::Point & Enemy::Graph::getPointAtCurrentPosition(const sf::Vector2f& entityCentrePosition, int tileSize) const
{
	const auto* pointAtCurrentPosition = getPointOnGraph(sf::Vector2f(std::floor(entityCentrePosition.x / tileSize),
		std::floor(entityCentrePosition.y / tileSize)));

	assert(pointAtCurrentPosition);
	return *pointAtCurrentPosition;
}

void Enemy::Graph::createGraph(const sf::Vector2f& entityCentrePosition, int tileSize)
{
	const sf::Vector2f startingPoint = sf::Vector2f(std::floor(entityCentrePosition.x / tileSize), std::floor(entityCentrePosition.y / tileSize));
	int pointID = 0;
	std::deque<Point> frontier;
	addNewPoint(startingPoint, frontier, pointID, pointID);

	while (!frontier.empty())
	{
		addNeighbouringPointsToFrontier(frontier.front(), frontier, pointID, tileSize);
		frontier.pop_front();
	}
}

const std::vector<Enemy::Point>& Enemy::Graph::getGraph() const
{
	return m_graph;
}

bool Enemy::Graph::isPowerUpFound() const
{
	return m_powerUpFound;
}

bool Enemy::Graph::isOpponentFound() const
{
	return m_opponentFound;
}

const sf::Vector2f & Enemy::Graph::getPowerUpAtPoint() const
{
	return m_powerUpAtPoint;
}

const sf::Vector2f & Enemy::Graph::getOpponentAtPoint() const
{
	return m_opponentAtPoint;
}

void Enemy::Graph::addNeighbouringPointsToFrontier(const Point& point, std::deque<Point>& frontier, int& pointID, int tileSize)
{
	for (int x = point.m_point.x - 1; x <= point.m_point.x + 1; x += 2)
	{
		if (CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(x, point.m_point.y), tileSize))
		{
			continue;
		}

		if (isPointOnGraph(m_graph, sf::Vector2f(x, point.m_point.y)))
		{
			continue;
		}

		if (!m_opponentFound && CollisionHandler::isEntityAtPosition(sf::Vector2f(x, point.m_point.y), m_entityManager, EntityTag::Player, tileSize))
		{
			m_opponentFound = true;
			m_opponentAtPoint = sf::Vector2f(x, point.m_point.y);
		}

		if (!m_powerUpFound && CollisionHandler::isEntityAtPosition(sf::Vector2f(x, point.m_point.y), m_entityManager, EntityTag::PowerUp, tileSize))
		{
			m_powerUpFound = true;
			m_powerUpAtPoint = sf::Vector2f(x, point.m_point.y);
		}

		addNewPoint(sf::Vector2f(x, point.m_point.y), frontier, pointID, point.m_ID);
	}

	//y
	for (int y = point.m_point.y - 1; y <= point.m_point.y + 1; y += 2)
	{
		if (CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(point.m_point.x, y), tileSize))
		{
			continue;
		}

		if (isPointOnGraph(m_graph, sf::Vector2f(point.m_point.x, y)))
		{
			continue;
		}

		if (!m_opponentFound && CollisionHandler::isEntityAtPosition(sf::Vector2f(point.m_point.x, y), m_entityManager, EntityTag::Player, tileSize))
		{
			m_opponentFound = true;
			m_opponentAtPoint = sf::Vector2f(point.m_point.x, y);
		}

		if (!m_powerUpFound && CollisionHandler::isEntityAtPosition(sf::Vector2f(point.m_point.x, y), m_entityManager, EntityTag::PowerUp, tileSize))
		{
			m_powerUpFound = true;
			m_powerUpAtPoint = sf::Vector2f(point.m_point.x, y);
		}

		addNewPoint(sf::Vector2f(point.m_point.x, y), frontier, pointID, point.m_ID);
	}
}

void Enemy::Graph::addNewPoint(const sf::Vector2f & position, std::deque<Point>& frontier, int & pointID, int cameFromID)
{
	const Point newPoint(position, pointID, cameFromID);
	frontier.push_back(newPoint);
	m_graph.push_back(newPoint);
	++pointID;
}


//Enemy
Enemy::Enemy(const std::string& name, EntityTag tag, const sf::Vector2f & position, EntityManager & entityManager, int entityID, bool collidable)
	: BombCarrier(name, tag, position, entityManager, entityID, collidable),
	m_stopMovementTimer(2.0f, false),
	m_targetPoint(),
	m_reachedTargetPoint(false),
	m_state(State::AssignNewState),
	m_chooseNewMoveDirection(true),
	m_walkingPoint(),
	m_moveErrorTimer(12.0f, true)
{
	m_speed = sf::Vector2f(22.5f, 22.5f);

	const int randNumb = RandomNumberGenerator::getRandomNumber(static_cast<int>(EnemyType::Passive), static_cast<int>(EnemyType::Aggressive));
	m_type = static_cast<EnemyType>(randNumb);

	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&Enemy::setStateToTargetPlayer, this), "Enemy", GameEvent::EnemyAggressive);
	gameEventMessenger.broadcast(GameEvent::EnemySpawned);
	m_type = EnemyType::Aggressive;
	
	auto& entityMessenger = EntityMessengerLocator::getEntityMessenger();
	entityMessenger.subscribe(std::bind(&Enemy::setTypeToAggressive, this, std::placeholders::_1), "Enemy", EntityEvent::TurnAggressive);
}

Enemy::~Enemy()
{
	AudioPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::EnemyDeath);
	GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::EnemyDeath);

	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("Enemy", GameEvent::EnemyAggressive);

	auto& entityMessenger = EntityMessengerLocator::getEntityMessenger();
	entityMessenger.unsubscribe("Enemy", EntityEvent::TurnAggressive);
}

void Enemy::handleEntityCollision(const std::unique_ptr<Entity>& entity, const sf::FloatRect& intersection)
{
	if (!m_lives)
	{
		return;
	}

	Character::handleEntityCollision(entity, intersection);

	if (entity->getTag() == EntityTag::PowerUp)
	{
		if (entity->getName() == "PowerUpIncreaseBombPower")
		{
			BombCarrier::increaseBombPower();
		}
		else if (entity->getName() == "PowerUpSpeedBoost")
		{
			BombCarrier::increaseSpeed(2.5f, 2.5f);
		}
	}
}

void Enemy::update(float deltaTime)
{
	BombCarrier::update(deltaTime);
	m_stopMovementTimer.update(deltaTime);

	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	Graph graph(Entity::getCentrePosition(), m_entityManager, tileSize);
	
	updateLocalHazardousPoints(graph, tileSize);
	handleStates(graph, tileSize);
	handleMovement(graph, deltaTime, tileSize);
}

void Enemy::setStateToTargetPlayer()
{
	setState(State::SetTargetPointAtOpponent);
}

void Enemy::handleStates(const Graph& graph, int tileSize)
{
	switch (m_state)
	{
	case State::AssignNewState :
	{
		if (graph.isOpponentFound() && m_type == EnemyType::Aggressive)
		{
			setTargetPointAtOpponent(graph, tileSize);
			break;
		}

		(graph.isPowerUpFound() ? setTargetPointAtPowerUp(graph.getPowerUpAtPoint()) : setTargetPointBesideCrate(graph, tileSize));
		break;
	}
	case State::SetTargetPointAtOpponent:
	{
		setTargetPointAtOpponent(graph, tileSize);
		break;
	}
	case State::SetTargetPointBesideCrate:
	{
		setTargetPointBesideCrate(graph, tileSize);
		break;
	}
	case State::SetTargetPointAtSafePoint :
	{
		setTargetPointAtSafePoint(graph, tileSize);
		break;
	}
	case State::StopMovement:
	{
		if (!m_stopMovementTimer.isActive())
		{
			m_stopMovementTimer.activate();
		}

		if (m_stopMovementTimer.isExpired())
		{
			m_stopMovementTimer.deactivate();
			m_stopMovementTimer.reset();
			setState(State::AssignNewState);
		}
		break;
	}
	case State::SetTargetPointAtPowerUp :
	{
		setTargetPointAtPowerUp(graph.getPowerUpAtPoint());
		break;
	}
	}
}

void Enemy::updateLocalHazardousPoints(const Graph& graph, int tileSize)
{	
	//Handle existing hazardous points
	for (auto iter = m_localHazardousPoints.begin(); iter != m_localHazardousPoints.end();)
	{
		if (!CollisionHandler::isEntityAtPosition(*iter, m_entityManager, EntityTag::Bomb, tileSize))
		{
			iter = m_localHazardousPoints.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	//search for new bombs
	bool newBombDiscovered = false;
	const auto& currentPointAtPosition = graph.getPointAtCurrentPosition(Entity::getCentrePosition(), tileSize).m_point;
	for (const auto neighbouringPoint : graph.getNeighbouringPointsOnGraphContainingBomb(currentPointAtPosition, tileSize))
	{
		auto cIter = std::find_if(m_localHazardousPoints.cbegin(), m_localHazardousPoints.cend(), 
			[&neighbouringPoint](const auto& point) { return point == neighbouringPoint; });

		if (cIter == m_localHazardousPoints.cend())
		{
			m_localHazardousPoints.emplace_back(neighbouringPoint);
			newBombDiscovered = true;
			m_stopMovementTimer.reset();
			m_stopMovementTimer.deactivate();
			break;
		}
	}

	if (newBombDiscovered)
	{
		setState(State::SetTargetPointAtSafePoint);
	}
}

void Enemy::assignNewMovementDirection(const sf::Vector2f& nextPointToMoveTo, int tileSize)
{
	const auto currentPosition = sf::Vector2f(std::floor(Entity::getCentrePosition().x / tileSize), std::floor(Entity::getCentrePosition().y / tileSize));
	if (nextPointToMoveTo.x > currentPosition.x)
	{
		m_currentMoveDirection = Direction::Right;
	}
	else if (nextPointToMoveTo.x < currentPosition.x)
	{
		m_currentMoveDirection = Direction::Left;
	}
	else if (nextPointToMoveTo.y < currentPosition.y)
	{
		m_currentMoveDirection = Direction::Up;
	}
	else if (nextPointToMoveTo.y > currentPosition.y)
	{
		m_currentMoveDirection = Direction::Down;
	}
}

void Enemy::setTargetPointAtOpponent(const Graph& graph, int tileSize)
{
	const auto neighbouringPoints = graph.getNeighbouringPointsFromPoint(graph.getOpponentAtPoint(), tileSize);
	if (neighbouringPoints.empty())
	{
		setState(State::StopMovement);
		return;
	}

	const sf::Vector2f randomPointPosition = neighbouringPoints[RandomNumberGenerator::getRandomNumber(0, neighbouringPoints.size() - 1)];
	setNewTargetPoint(randomPointPosition);
	setState(State::TargettingOpponent);
}

void Enemy::setTargetPointBesideCrate(const Graph& graph, int tileSize)
{
	const int maximumCratesToChoose = 3;
	int cratesToChoose = maximumCratesToChoose;
	std::vector<int> neighbouringPointsOnCrate;
	for (const auto& point : graph.getGraph())
	{
		const auto neighbouringPointsOnCrates = graph.getNeighbouringPointsOnEntity(point.m_point, tileSize, EntityTag::Crate);
		if (!neighbouringPointsOnCrates.empty())
		{
			--cratesToChoose;
			neighbouringPointsOnCrate.push_back(point.m_ID);
			if (neighbouringPointsOnCrate.size() == cratesToChoose)
			{
				break;
			}
		}
	}

	//No more crates to choose, target the opponent instead
	if (cratesToChoose == maximumCratesToChoose)
	{
		setState(State::SetTargetPointAtOpponent);
		return;
	}

	const auto& randomPointID = neighbouringPointsOnCrate[RandomNumberGenerator::getRandomNumber(0, neighbouringPointsOnCrate.size() - 1)];
	setNewTargetPoint(graph.getGraph()[randomPointID].m_point);
	m_state = State::TargettingCrate;
}

void Enemy::assignPointsToMoveTo(const Graph& graph, std::deque<sf::Vector2f>& pointsToMoveTo, int tileSize)
{	
	const auto* moveToPoint = graph.getPointOnGraph(m_targetPoint);
	if (!moveToPoint)
	{
		setState(State::AssignNewState);
		return;
	}

	const auto& pointAtPosition = graph.getPointAtCurrentPosition(Entity::getCentrePosition(), tileSize);
	pointsToMoveTo.emplace_front(moveToPoint->m_point);
	while (moveToPoint->m_point != pointAtPosition.m_point)
	{
		const int nextPointID = moveToPoint->m_cameFromID;
		const auto& nextPoint = *graph.getPointOnGraph(nextPointID);

		if (nextPoint.m_point == pointAtPosition.m_point)
		{
			break;
		}
		pointsToMoveTo.emplace_front(nextPoint.m_point);
		moveToPoint = graph.getPointOnGraph(nextPointID);
	}

	assert(!pointsToMoveTo.empty());
}

void Enemy::setNewTargetPoint(const sf::Vector2f & point)
{
	m_targetPoint = point;
	m_reachedTargetPoint = false;
}

bool Enemy::neighbouringCrateAtPoint(const std::vector<sf::Vector2f>& points, int tileSize) const
{
	for (const auto& point : points)
	{
		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(point.x, point.y), m_entityManager, EntityTag::Crate, tileSize))
		{
			return true;
		}
	}

	return false;
}

void Enemy::setTargetPointAtSafePoint(const Graph& graph, int tileSize)
{
	bool pointSafe = false;
	const int maxSafePoints = 2;
	std::vector<int> safePointsID;
	int pointID = 0;

	for (const auto& point : graph.getGraph())
	{
		if (isPointSafeFromBombsAtPoint(point.m_point, tileSize))
		{
			safePointsID.push_back(point.m_ID);
			if (safePointsID.size() == maxSafePoints)
			{
				break;
			}
		}		
	}

	if (!safePointsID.empty())
	{
		const int randPointID = RandomNumberGenerator::getRandomNumber(0, safePointsID.size() - 1);	
		const auto& newTargetPoint = graph.getPointOnGraph(safePointsID[randPointID])->m_point;
		setNewTargetPoint(newTargetPoint);
		setState(State::MovingToSafePoint);
	}
	else
	{
		setState(State::StopMovement);
	}
}

bool Enemy::isPointSafeFromBombsAtPoint(const sf::Vector2f& point, int tileSize) const
{
	for (const auto& bombAtPoint : m_localHazardousPoints)
	{
		if (point.x != bombAtPoint.x && point.y != bombAtPoint.y)
		{
			return true;
		}
	}

	return false;
}

void Enemy::setState(State newState)
{
	m_state = newState;
}

void Enemy::setTypeToAggressive(EntityMessage & entityMessage)
{
	if (m_type != EnemyType::Aggressive)
	{
		m_type = EnemyType::Aggressive;
		entityMessage.decreaseCharge();
	}
}

void Enemy::setTargetPointAtPowerUp(const sf::Vector2f & powerUpPoint)
{
	setNewTargetPoint(powerUpPoint);
	setState(State::TargettingPowerUp);
}

bool Enemy::reachedTargetPoint(const Graph& graph, int tileSize) const
{
	return graph.getPointAtCurrentPosition(Entity::getCentrePosition(), tileSize).m_point == m_targetPoint;
	//return (getPointAtCurrentPosition(graph, tileSize).m_point == m_targetPoint);	
}

void Enemy::handleMovement(const Graph& graph, float deltaTime, int tileSize)
{
	if (m_stopMovementTimer.isActive())
	{
		return;
	}

	m_moveErrorTimer.update(deltaTime);
	handleNewMoveDirection(graph, tileSize);
	move(graph, deltaTime, tileSize);

	//Hack - Fixes issue with getting stuck
	if (m_moveErrorTimer.isExpired())
	{
		const auto currentPosition = Entity::getPosition();
		//const sf::Vector2f pointAtCurrentPosition = sf::Vector2f(getPointAtCurrentPosition(graph, tileSize).m_point.x * tileSize, 
		//	getPointAtCurrentPosition(graph, tileSize).m_point.y * tileSize);
		//m_position = sf::Vector2f(pointAtCurrentPosition.x * tileSize, pointAtCurrentPosition.y * tileSize);
		//m_moveErrorTimer.reset();
		//m_chooseNewMoveDirection = true;
		//std::cout << "Unstuck\n";
	}

	if (m_reachedTargetPoint)
	{
		if (m_state == State::TargettingCrate || m_state == State::TargettingOpponent)
		{
			BombCarrier::placeBomb();
		}

		if (m_state == State::MovingToSafePoint)
		{
			setState(State::StopMovement);
		}

		if (m_state == State::TargettingPowerUp)
		{
			setState(State::AssignNewState);
		}

		m_reachedTargetPoint = true;
	}
}

void Enemy::assignWalkingPoint(const Graph& graph, const std::deque<sf::Vector2f>& pointsToMoveTo, int tileSize)
{
	const auto& currentPoint = graph.getPointAtCurrentPosition(Entity::getCentrePosition(), tileSize).m_point;
	if (currentPoint.x == m_targetPoint.x || currentPoint.y == m_targetPoint.y)
	{
		bool clearPathToTargetPoint = true;

		//Handle x axis
		if (currentPoint.x == m_targetPoint.x)
		{
			if (currentPoint.y > m_targetPoint.y)
			{
				for (int y = currentPoint.y; y > m_targetPoint.y; --y)
				{
					const auto position = sf::Vector2f(currentPoint.x, y);
					if (CollisionHandler::isCollidableTileAtPosition(position, tileSize))
					{
						clearPathToTargetPoint = false;
						break;
					}
				}
			}
			else if (currentPoint.y < m_targetPoint.y)
			{
				for (int y = currentPoint.y; y < m_targetPoint.y; ++y)
				{
					const auto position = sf::Vector2f(currentPoint.x, y);
					if (CollisionHandler::isCollidableTileAtPosition(position, tileSize))
					{
						clearPathToTargetPoint = false;
						break;
					}
				}
			}
		}


		//Handle y axis
		if (currentPoint.y == m_targetPoint.y)
		{
			if (currentPoint.x < m_targetPoint.x)
			{
				for (int x = currentPoint.x; x < m_targetPoint.x; x++)
				{
					const auto position = sf::Vector2f(x, currentPoint.y);
					if (CollisionHandler::isCollidableTileAtPosition(position, tileSize))
					{
						clearPathToTargetPoint = false;
						break;
					}
				}
			}
			else if (currentPoint.x > m_targetPoint.x)
			{
				for (int x = currentPoint.x; x > m_targetPoint.x; x--)
				{
					const auto position = sf::Vector2f(x, currentPoint.y);
					if (CollisionHandler::isCollidableTileAtPosition(position, tileSize))
					{
						clearPathToTargetPoint = false;
						break;
					}
				}
			}
		}

		if (clearPathToTargetPoint)
		{
			m_walkingPoint = m_targetPoint;
			return;
		}
	}

	//Find node that is just before change of axis relative to enemy
	const sf::Vector2f* previousPoint = nullptr;
	for (auto cIter = pointsToMoveTo.cbegin(); cIter != pointsToMoveTo.cend(); ++cIter)
	{
		if (cIter == pointsToMoveTo.cbegin())
		{
			previousPoint = &(*cIter);
			continue;
		}

		if (cIter->x != currentPoint.x && cIter->y != currentPoint.y)
		{
			m_walkingPoint = *previousPoint;
			break;
		}

		previousPoint = &(*cIter);
	}
}

void Enemy::handleNewMoveDirection(const Graph& graph, int tileSize)
{
	if (m_chooseNewMoveDirection)
	{
		//If target is no longer at target location
		if (!graph.isTargetAtTargetPoint(m_targetPoint, m_state, tileSize) && m_state != State::MovingToSafePoint && m_state != State::TargettingOpponent)
		{
			setState(State::AssignNewState);
			return;
		}

		//Scan immediate surrounding area
		//If next point is only path to go, go anyway
		std::deque<sf::Vector2f> pointsToMoveTo; 
		assignPointsToMoveTo(graph, pointsToMoveTo, tileSize);
		if (pointsToMoveTo.empty())
		{
			return;
		}
		m_walkingPoints = pointsToMoveTo;

		//If next tile to walk over is in radius of harm, cease movement
		if (graph.isPointInRadiusOfHarm(pointsToMoveTo.front(), tileSize) && m_state != State::MovingToSafePoint)
		{
			setState(State::StopMovement);
			return;
		}

		assignWalkingPoint(graph, pointsToMoveTo, tileSize);
		assignNewMovementDirection(pointsToMoveTo.front(), tileSize);
		m_chooseNewMoveDirection = false;
	}
}

void Enemy::move(const Graph& graph, float deltaTime, int tileSize)
{
	const sf::FloatRect walkingPointAABB(sf::Vector2f(m_walkingPoint.x * tileSize, m_walkingPoint.y * tileSize), sf::Vector2f(tileSize, tileSize));
	const auto centrePosition = Entity::getCentrePosition();
	const sf::FloatRect AABB(m_position, sf::Vector2f(tileSize, tileSize));
	bool correctPosition = false;

	//Calculate intersection froim next tile
	switch (m_currentMoveDirection)
	{
	case Direction::Right:
	{
		if (m_position.x >= (walkingPointAABB.left) && AABB.intersects(walkingPointAABB))
		{
			correctPosition = true;
			break;
		}

		m_velocity.x += m_speed.x * deltaTime;
		break;
	}
	case Direction::Left:
	{
		if (m_position.x <= walkingPointAABB.left && AABB.intersects(walkingPointAABB))
		{
			correctPosition = true;
			break;
		}

		m_velocity.x -= m_speed.x * deltaTime;
		break;
	}
	case Direction::Up:
	{
		if (m_position.y <= (walkingPointAABB.top) && AABB.intersects(walkingPointAABB))
		{
			correctPosition = true;
			break;
		}
		m_velocity.y -= m_speed.y * deltaTime;
		break;
	}
	case Direction::Down:
	{
		if (m_position.y >= walkingPointAABB.top && AABB.intersects(walkingPointAABB))
		{
			correctPosition = true;
			break;
		}
		m_velocity.y += m_speed.y * deltaTime;
		break;
	}
	}
	
	if (correctPosition)
	{
		m_moveErrorTimer.reset();
		m_position = sf::Vector2f(m_walkingPoint.x * tileSize, m_walkingPoint.y * tileSize);
		if (reachedTargetPoint(graph, tileSize))
		{
			m_reachedTargetPoint = true;
		}

		m_chooseNewMoveDirection = true;
	}
}