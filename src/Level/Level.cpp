#include "Level.h"
#include <utility>
#include <algorithm>
#include <assert.h>
#include <math.h>

Level::Level(std::vector<TileLayer>&& tileLayers, const std::string& levelName, const sf::Vector2i& levelSize, int tileSize)
	: m_tileLayers(std::move(tileLayers)),
	m_levelName(levelName),
	m_levelSize(levelSize),
	m_tileSize(tileSize)
{}

void Level::draw(sf::RenderWindow& window)
{
	for (auto& tileLayer : m_tileLayers)
	{
		tileLayer.draw(window);
	}
}

const sf::Vector2i & Level::getSize() const
{
	return m_levelSize;
}

const std::string & Level::getName() const
{
	return m_levelName;
}

int Level::getTileSize() const
{
	return m_tileSize;
}