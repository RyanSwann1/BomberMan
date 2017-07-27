#include "AILogic.h"
#include <deque>
#include <Level\Level.h>
#include <math.h>
#include <Entities\CollisionHandler.h>
#include <Game\DebugOverlay.h>
#include <assert.h>

class Node
{
public:
	Node(const sf::Vector2i& position, int cameFromID)
		: m_position(position),
		m_cameFromID(cameFromID)
	{}

	const sf::Vector2i m_position;
	const int m_cameFromID;
};

std::vector<Node> getNeighbouringNodes(const sf::Vector2i& position, const std::unique_ptr<Level>& currentLevel);
void addToGraph(const Node& node);

std::vector<GraphPosition> m_graph;
std::deque<Node> m_frontier;
int m_graphPositionID = 0;

const std::vector<GraphPosition>& AILogic::getGraph()
{
	return m_graph;
}

void AILogic::initializeGraph(const std::unique_ptr<Level>& currentLevel, const sf::Vector2i& startingPosition)
{
	//Initialize start of frontier
	const int tileSize = currentLevel->getTileSize();
	m_frontier.emplace_back(startingPosition, m_graphPositionID);

	while (!m_frontier.empty())
	{
		const auto& node = m_frontier.front();
		addToGraph(node);
		const auto neighbourPositions = getNeighbouringNodes(node.m_position, currentLevel);
		for (const auto& neighbourPosition : neighbourPositions)
		{
			m_frontier.push_back(neighbourPosition);
		}

		m_frontier.pop_front();
	}
}

const GraphPosition & AILogic::getPointOnGraph(const sf::Vector2i & position)
{
	auto cIter = std::find_if(m_graph.cbegin(), m_graph.cend(), [&position](const auto& graphPosition) {return graphPosition.m_position == position; });
	assert(cIter != m_graph.cend());
	return (*cIter);
}

std::vector<Node> getNeighbouringNodes(const sf::Vector2i & position, const std::unique_ptr<Level>& currentLevel)
{
	std::vector<Node> neighbourPositions;
	//Search x axis
	for (int x = position.x - 1; x <= position.x + 1; ++x)
	{
		if (x == position.x)
		{
			continue;
		}

		if (CollisionHandler::isCollidableTileAtPosition(sf::Vector2i(x, position.y)))
		{
			continue;
		}
		
		if (std::find_if(m_graph.cbegin(), m_graph.cend(), [x, &position](const auto& graphPosition)
			{ return graphPosition.m_position == sf::Vector2i(x, position.y); }) != m_graph.cend())
		{
			continue;
		}

		auto cIter = std::find_if(m_graph.cbegin(), m_graph.cend(), [&position](const auto& graphPosition) { return graphPosition.m_position == position; });
		assert(cIter != m_graph.cend());

		neighbourPositions.emplace_back(sf::Vector2i(x, position.y), cIter->m_ID);
	}

	//Search y axis
	for (int y = position.y - 1; y <= position.y + 1; ++y)
	{
		if (y == position.y)
		{
			continue;
		}

		if (CollisionHandler::isCollidableTileAtPosition(sf::Vector2i(position.x, y)))
		{
			continue;
		}

		if (std::find_if(m_graph.cbegin(), m_graph.cend(), [&position, y](const auto& graphPosition)
		{ return graphPosition.m_position == sf::Vector2i(position.x, y); }) != m_graph.cend())
		{
			continue;
		}

		auto cIter = std::find_if(m_graph.cbegin(), m_graph.cend(), [&position](const auto& graphPosition) { return graphPosition.m_position == position; });
		assert(cIter != m_graph.cend());

		neighbourPositions.emplace_back(sf::Vector2i(position.x, y), cIter->m_ID);
	}

	return neighbourPositions;
}

void addToGraph(const Node & node)
{
	auto cIter = std::find_if(m_graph.cbegin(), m_graph.cend(), [&node](const auto& graphPosition) { return graphPosition.m_position == node.m_position; });
	if (cIter == m_graph.cend())
	{
		++m_graphPositionID;
		m_graph.emplace_back(node.m_position, m_graphPositionID, node.m_cameFromID);
		//DebugOverlay::addRect(sf::Vector2f(node.m_position.x, node.m_position.y));
	}
}

const GraphPosition & AILogic::getPointOnGraph(int ID)
{
	auto cIter = std::find_if(m_graph.cbegin(), m_graph.cend(), [ID](const auto& pointOnGraph) { return pointOnGraph.m_ID == ID; });
	assert(cIter != m_graph.cend());
	return (*cIter);
}