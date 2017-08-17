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
#include <random>

Enemy::Enemy(const std::string& name, EntityTag tag, const sf::Vector2f & position, EntityManager & entityManager, int entityID, bool collidable)
	: BombCarrier(name, tag, position, entityManager, entityID, collidable),
	m_movementTimer(0.75f, true),
	m_stopMovementTimer(2.0f, false),
	m_bombScannerTimer(1.0f, false),
	m_targetPoint(),
	m_reachedTargetPoint(false),
	m_bombAtPoints(),
	m_state(State::InitializeState)
{
	const int randNumb = RandomNumberGenerator::getRandomNumber(static_cast<int>(EnemyType::Passive), static_cast<int>(EnemyType::Aggressive));
	m_type = static_cast<EnemyType>(randNumb);

	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	void setStateToTargetPlayer();
	gameEventMessenger.subscribe(std::bind(&Enemy::setStateToTargetPlayer, this), "Enemy", GameEvent::EnemyAggressive);
	gameEventMessenger.broadcast(GameEvent::EnemySpawned);
	
	auto& entityMessenger = EntityMessengerLocator::getEntityMessenger();
	void setTypeToAggressive(EntityMessage& entityMessage);
	entityMessenger.subscribe(std::bind(&Enemy::setTypeToAggressive, this, std::placeholders::_1), "Enemy", EntityEvent::TurnAggressive);
}

Enemy::~Enemy()
{
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
	if (!m_lives)
	{
		AudioPlayerLocator::getAudioClipPlayer().playAudioClip(AudioClipName::EnemyDeath);
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::EnemyDeath);
	}

	switch (entity->getTag())
	{
	case EntityTag::PowerUpSpeedBoost :
	{
		
		break;
	}
	}
}

void Enemy::update(float deltaTime)
{
	BombCarrier::update(deltaTime);
	m_stopMovementTimer.update(deltaTime);
	m_movementTimer.update(deltaTime);
	if (!m_movementTimer.isExpired())
	{
		return;
	}

	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	std::vector<Point> graph;
	bool opponentFound = false;
	sf::Vector2i opponentAtPoint;

	initializeGraph(opponentAtPoint, graph, opponentFound, tileSize);
	handleBombAtPoints(graph, tileSize);
	handleStates(graph, opponentAtPoint, opponentFound, tileSize);

	if (!m_reachedTargetPoint)
	{
		moveToTargetPoint(graph, tileSize);
		if (reachedTargetPoint(graph, tileSize))
		{
			if (m_state != State::MovingToSafePoint)
			{
				placeBomb();
				setState(State::MovingToSafePoint);
				m_bombScannerTimer.reset();
				return;
			}
			
			if (m_state == State::MovingToSafePoint)
			{
				setState(State::StopMovement);
			}
		}
	}
}

void Enemy::setStateToTargetPlayer()
{
	setState(State::TargetOpponent);
}

void Enemy::checkExistingBombsAtPoints(int tileSize)
{
	for (auto iter = m_bombAtPoints.begin(); iter != m_bombAtPoints.end();)
	{
		//const auto point = sf::Vector2f(iter->x * tileSize, iter->y * tileSize);
		if (!CollisionHandler::isEntityAtPosition(sf::Vector2f(iter->x, iter->y), m_entityManager, EntityTag::Bomb, tileSize))
		{
			iter = m_bombAtPoints.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Enemy::move(const std::vector<Point>& graph, const Point& point, int tileSize)
{
	switch (m_state)
	{
	case State::TargettingCrates :
	{
		const bool crateAtTargetPoint = isTargetNeighbouringTargetPoint(graph, EntityTag::Crate, tileSize);
		if (!crateAtTargetPoint)
		{
			setState(State::setTargetPointAtCrate);
			return;
		}

		//Check point to see if its near bomb or explosion
		if (isPointInRadiusOfHarm(graph, point, tileSize))
		{
			setState(State::StopMovement);
			return;
		}
		break;
	}
	}

	Entity::setPosition(point.m_point.x * tileSize, point.m_point.y * tileSize);
	m_movementTimer.reset();
	m_movementTimer.activate();
}

void Enemy::handleStates(const std::vector<Point>& graph, const sf::Vector2i & opponentAtPoint, bool opponentFound, int tileSize)
{
	switch (m_state)
	{
	case State::InitializeState :
	{
		if (m_type == EnemyType::Aggressive)
		{
			(opponentFound ? setState(State::TargetOpponent) : setTargetPointAtCrate(graph, tileSize));
		}
		else if (m_type == EnemyType::Passive)
		{
			setTargetPointAtCrate(graph, tileSize);	
		}
		break;
	}
	case State::TargetOpponent:
	{
		//Temporary fix to issue I'll fix
		if (opponentAtPoint == sf::Vector2i(0, 0))
		{
			setNewTargetPoint(graph[0].m_point);
			break;
		}

		const auto& neighbouringPoint = getNeighbouringPointOnGraph(opponentAtPoint, graph, tileSize);
		setNewTargetPoint(neighbouringPoint);
		break;
	}
	case State::setTargetPointAtCrate:
	{
		setTargetPointAtCrate(graph, tileSize);
		break;
	}
	case State::setTargetPointAtSafePoint :
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
			setState(State::InitializeState);
		}
		break;
	}
	}
}

void Enemy::handleBombAtPoints(const std::vector<Point>& graph, int tileSize)
{
	if (m_stopMovementTimer.isActive())
	{
		return;
	}
		
	checkExistingBombsAtPoints(tileSize);

	for (const auto neighbouringPoint : getNeighbouringPointsOnGraphContainingBomb(getCurrentPoint(graph, tileSize).m_point, graph, tileSize))
	{
		auto cIter = std::find_if(m_bombAtPoints.cbegin(), m_bombAtPoints.cend(), [&neighbouringPoint](const auto& point) { return point == neighbouringPoint; });
		if (cIter == m_bombAtPoints.cend())
		{
			m_bombAtPoints.push_back(neighbouringPoint);
			setState(State::setTargetPointAtSafePoint);
			m_stopMovementTimer.reset();
			m_stopMovementTimer.deactivate();
		}
	}
}

void Enemy::initializeGraph(sf::Vector2i& opponentAtPoint, std::vector<Point>& graph, bool& opponentFound, int tileSize)
{
	const sf::Vector2i startingPoint(std::floor(m_position.x / tileSize), std::floor(m_position.y / tileSize));
	int pointID = 0;
	std::deque<Point> frontier;
	addNewPoint(startingPoint, graph, frontier, pointID, pointID);

	while (!frontier.empty())
	{
		addNeighbouringPointsToFrontier(opponentAtPoint, frontier.front(), graph, frontier, pointID, opponentFound, tileSize);
		frontier.pop_front();
	}
}

void Enemy::addNeighbouringPointsToFrontier(sf::Vector2i& opponentAtPoint, const Point& point, std::vector<Point>& graph, std::deque<Point>& frontier, int& pointID, bool& opponentFound, int tileSize)
{
	//x
	for (int x = point.m_point.x - 1; x <= point.m_point.x + 1; x += 2)
	{
		if (CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(x, point.m_point.y), tileSize))
		{
			continue;
		}

		if (isPointOnGraph(graph, sf::Vector2i(x, point.m_point.y)))
		{
			continue;
		}

		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(x, point.m_point.y), m_entityManager, EntityTag::Player, tileSize))
		{
			opponentFound = true;
			opponentAtPoint = sf::Vector2i(x, point.m_point.y);
			addNewPoint(sf::Vector2i(x, point.m_point.y), graph, frontier, pointID, point.m_ID);
			break;
		}

		addNewPoint(sf::Vector2i(x, point.m_point.y), graph, frontier, pointID, point.m_ID);
	}

	//y
	for (int y = point.m_point.y - 1; y <= point.m_point.y + 1; y += 2)
	{
		if (CollisionHandler::isCollidableTileAtPosition(sf::Vector2f(point.m_point.x, y), tileSize))
		{
			continue;
		}

		if (isPointOnGraph(graph, sf::Vector2i(point.m_point.x, y)))
		{
			continue;
		}

		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(point.m_point.x, y), m_entityManager, EntityTag::Player, tileSize))
		{
			opponentFound = true;
			opponentAtPoint = sf::Vector2i(point.m_point.x, y);
			addNewPoint(sf::Vector2i(point.m_point.x, y), graph, frontier, pointID, point.m_ID);
			break;
		}

		//if (CollisionHandler::isEntityAtPosition(EntityTag::Enemy, sf::Vector2f(point.m_position.x * tileSize, y * tileSize), m_entityManager))
		//{
		//	opponentFound = true;
		//	addNewPoint(sf::Vector2i(point.m_position.x, y), graph, frontier, pointID, point.m_ID);
		//	break;
		//}

		addNewPoint(sf::Vector2i(point.m_point.x, y), graph, frontier, pointID, point.m_ID);		
	}
}

std::vector<sf::Vector2i> Enemy::getNeighbouringPointsOnGraph(const sf::Vector2i& startingPoint, const std::vector<Point>& graph, int tileSize) const
{
	std::vector<sf::Vector2i> neighbouringPoints;
	for (int x = startingPoint.x - 1; x <= startingPoint.x + 1; x += 2)
	{
		if (!isPointOnGraph(graph, sf::Vector2i(x, startingPoint.y)))
		{
			continue;
		}

		neighbouringPoints.emplace_back(x, startingPoint.y);
	}

	for (int y = startingPoint.y - 1; y <= startingPoint.y + 1; y += 2)
	{
		if (!isPointOnGraph(graph, sf::Vector2i(startingPoint.x, y)))
		{
			continue;
		}

		neighbouringPoints.emplace_back(startingPoint.x, y);
	}

	return neighbouringPoints;
}

std::vector<sf::Vector2i> Enemy::getNeighbouringPointsOnCrates(const sf::Vector2i & startingPoint, int tileSize) const
{
	std::vector<sf::Vector2i> pointsWithCrates;
	for (int x = startingPoint.x - 1; x <= startingPoint.x + 1; x += 2)
	{
		if (!CollisionHandler::isEntityAtPosition(sf::Vector2f(x, startingPoint.y), m_entityManager, EntityTag::Crate, tileSize))
		{
			continue;
		}

		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(x, startingPoint.y), m_entityManager, EntityTag::Solid, tileSize))
		{
			continue;
		}

		pointsWithCrates.emplace_back(x, startingPoint.y);
	}

	for (int y = startingPoint.y - 1; y <= startingPoint.y + 1; y += 2)
	{
		if (!CollisionHandler::isEntityAtPosition(sf::Vector2f(startingPoint.x, y), m_entityManager, EntityTag::Crate, tileSize))
		{
			continue;
		}


		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(startingPoint.x, y), m_entityManager, EntityTag::Solid, tileSize))
		{
			continue;
		}

		pointsWithCrates.emplace_back(startingPoint.x, y);
	}

	return pointsWithCrates;
}

std::vector<sf::Vector2i> Enemy::getNeighbouringPointsOnGraphContainingBomb(const sf::Vector2i & startingPoint, const std::vector<Point>& graph, int tileSize) const
{
	std::vector<sf::Vector2i> points;
	//x
	for (int x = startingPoint.x - 5; x <= startingPoint.x + 5; ++x)
	{
		if (!isPointOnGraph(graph, sf::Vector2i(x, startingPoint.y)))
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
		if (!isPointOnGraph(graph, sf::Vector2i(startingPoint.x, y)))
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

void Enemy::addNewPoint(const sf::Vector2i & position, std::vector<Point>& graph, std::deque<Point>& frontier, int& pointID, int cameFromID)
{
	const Point newPoint(position, pointID, cameFromID);
	frontier.push_back(newPoint);
	graph.push_back(newPoint);
	++pointID;
}

const Enemy::Point& Enemy::getPointOnGraph(const std::vector<Point>& graph, const sf::Vector2i & position) const
{
	//if(m_position)
	auto cIter = std::find_if(graph.cbegin(), graph.cend(), [&position](const auto& point) { return point.m_point == position; });
	assert(cIter != graph.cend());
	return *cIter;
}

const Enemy::Point & Enemy::getPointOnGraph(const std::vector<Point>& graph, int ID) const
{
	return graph[ID];
}

const sf::Vector2i Enemy::getNeighbouringPointOnGraph(const sf::Vector2i & point, const std::vector<Point>& graph, int tileSize) const
{
	const auto neighbouringPoints = getNeighbouringPointsOnGraph(point, graph, tileSize);
	assert(!neighbouringPoints.empty());
	
	const auto randomPointPosition = neighbouringPoints[RandomNumberGenerator::getRandomNumber(0, neighbouringPoints.size() - 1)];
	return randomPointPosition;
}

bool Enemy::isPointInRadiusOfHarm(const std::vector<Point>& graph, const Point& point, int tileSize) const
{
	//x
	for (int x = point.m_point.x - 5; x != point.m_point.x + 5; ++x)
	{
		if (!isPointOnGraph(graph, point.m_point))
		{
			continue;
		}

		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(x, point.m_point.y), m_entityManager, EntityTag::Bomb, tileSize))
		{
			return true;
		}
	}

	//y
	for (int y = point.m_point.y - 5; y != point.m_point.y + 5; ++y)
	{
		if (!isPointOnGraph(graph, point.m_point))
		{
			continue;
		}

		if (CollisionHandler::isEntityAtPosition(sf::Vector2f(point.m_point.x, y), m_entityManager, EntityTag::Bomb, tileSize))
		{
			return true;
		}
	}

	return false;
}

void Enemy::setTargetPointAtCrate(const std::vector<Point>& graph, int tileSize)
{
	int cratesToChoose = 3;
	std::vector<int> neighbouringPointsOnCrate;
	for (const auto& point : graph)
	{
		const auto neighbouringPointsOnCrates = getNeighbouringPointsOnCrates(point.m_point, tileSize);
		for (const auto neighbouringPoint : neighbouringPointsOnCrates)
		{
			--cratesToChoose;
			neighbouringPointsOnCrate.push_back(point.m_ID);
			if (neighbouringPointsOnCrate.size() == cratesToChoose)
			{
				break;
			}
		}

		if (neighbouringPointsOnCrate.size() == cratesToChoose)
		{
			break;
		}
	}

	if (cratesToChoose == 3)
	{
		setState(State::TargetOpponent);
		return;
	}

	assert(!neighbouringPointsOnCrate.empty());
	const auto& randomPointID = neighbouringPointsOnCrate[RandomNumberGenerator::getRandomNumber(0, neighbouringPointsOnCrate.size() - 1)];
	setNewTargetPoint(graph[randomPointID].m_point);
	m_state = State::TargettingCrates;
}

void Enemy::moveToTargetPoint(const std::vector<Point>& graph, int tileSize)
{
	const auto* moveToPoint = &getPointOnGraph(graph, m_targetPoint);
	const auto currentPoint = getCurrentPoint(graph, tileSize);
	while (moveToPoint->m_point != currentPoint.m_point)
	{
		int nextPointID = moveToPoint->m_cameFromID;
		if (getPointOnGraph(graph, nextPointID).m_point == currentPoint.m_point)
		{
			move(graph, *moveToPoint, tileSize);
			break;
		}
		moveToPoint = &getPointOnGraph(graph, nextPointID);
	}

	if (getCurrentPoint(graph, tileSize).m_point == m_targetPoint)
	{
		m_reachedTargetPoint = true;
	}
}

void Enemy::setNewTargetPoint(const sf::Vector2i & point)
{
	m_targetPoint = point;
	m_reachedTargetPoint = false;
}

bool Enemy::neighbouringCrateAtPoint(const std::vector<sf::Vector2i>& points, int tileSize) const
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

void Enemy::setTargetPointAtSafePoint(const std::vector<Point>& graph, int tileSize)
{
	bool pointSafe = false;
	const int maxSafePoints = 5;
	std::vector<int> safePointsID;
	int pointID = 0;

	for (const auto& point : graph)
	{
		if (isPointSafeFromBombsAtPoint(graph, point.m_point, tileSize))
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
		const auto randPointID = RandomNumberGenerator::getRandomNumber(0, safePointsID.size() - 1);	
		setNewTargetPoint(getPointOnGraph(graph, safePointsID[randPointID]).m_point);
		setState(State::MovingToSafePoint);
	}
	else
	{
		setState(State::StopMovement);
	}
}

bool Enemy::isPointAppropriateDistanceAway(const std::vector<Point>& graph, const Point & requestedPoint, int tileSize) const
{
	const auto& currentPoint = getCurrentPoint(graph, tileSize);
	if (requestedPoint.m_point.x != currentPoint.m_point.x && requestedPoint.m_point.y != currentPoint.m_point.y)
	{
		return true;
	}

	int moveSpacesAway = 3;
	int nextPointID = requestedPoint.m_ID;
	while (nextPointID != currentPoint.m_ID)
	{
		const auto& nextPoint = graph[nextPointID];
		if (nextPoint.m_cameFromID == currentPoint.m_ID)
		{
			break;
		}
		nextPointID = nextPoint.m_cameFromID;
		--moveSpacesAway;
	}

	return (moveSpacesAway <= 0 ? true : false);
}

bool Enemy::isPointSafeFromBombsAtPoint(const std::vector<Point>& graph, const sf::Vector2i& point, int tileSize) const
{
	bool pointSafe = false;
	for (const auto& bombAtPoint : m_bombAtPoints)
	{
		if (point.x != bombAtPoint.x && point.y != bombAtPoint.y)
		{
			pointSafe = true;
		}
	}

	return pointSafe;
}

const Enemy::Point& Enemy::getCurrentPoint(const std::vector<Point>& graph, int tileSize) const
{
	return getPointOnGraph(graph, sf::Vector2i(std::floor(m_position.x / tileSize), std::floor(m_position.y / tileSize)));
}

void Enemy::setState(State newState)
{
	m_state = newState;
}

void Enemy::setTypeToAggressive(EntityMessage & entityMessage)
{
	if (m_type == EnemyType::Aggressive)
	{
		return;
	}

	m_type = EnemyType::Aggressive;
	entityMessage.decreaseCharge();
}

bool Enemy::reachedTargetPoint(const std::vector<Point>& graph, int tileSize) const
{
	return (getCurrentPoint(graph, tileSize).m_point == m_targetPoint);
}

bool Enemy::isTargetNeighbouringTargetPoint(const std::vector<Point>& graph, EntityTag entityTag, int tileSize) const
{
	switch (entityTag)
	{
	case EntityTag::Player :
	{
		const auto& playerPosition = GameLogic::getEntityPosition(m_entityManager, EntityTag::Player);
		const bool entityAtPosition = CollisionHandler::isEntityAtPosition(sf::Vector2f(std::floor(playerPosition.x / tileSize),
			std::floor(playerPosition.y / tileSize)), m_entityManager, EntityTag::Player, tileSize);

		return (entityAtPosition ? true : false);
	}
	case EntityTag::Crate :
	{
		return (getNeighbouringPointsOnCrates(m_targetPoint, tileSize).size() >= 0 ? true : false);
	}
	default:
		return false;
	}
}

bool Enemy::isPointOnGraph(const std::vector<Point>& graph, const sf::Vector2i & point) const
{
	auto cIter = std::find_if(graph.cbegin(), graph.cend(), [&point](const auto& pointOnGraph) { return pointOnGraph.m_point == point; });
	return (cIter != graph.cend() ? true : false);
}