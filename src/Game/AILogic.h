#pragma once

#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

class Level;
class GraphPosition
{
public:
	GraphPosition(const sf::Vector2i& position, int ID, int cameFromID)
		: m_position(position),
		m_ID(ID),
		m_cameFromID(cameFromID)
	{}

	const sf::Vector2i m_position;
	const int m_ID;
	const int m_cameFromID;
};

namespace AILogic
{
	const std::vector<GraphPosition>& getGraph();
	void initializeGraph(const std::unique_ptr<Level>& currentLevel, const sf::Vector2i& startingPosition);
	const GraphPosition& getPointOnGraph(const sf::Vector2i& position);
	const GraphPosition& getPointOnGraph(int ID);

}