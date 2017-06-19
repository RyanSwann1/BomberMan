#include "Level.h"
#include <utility>
#include <algorithm>

Level::Level(std::vector<TileLayer>&& tileLayers, std::vector<sf::Vector2i>&& collisionLayer,
	const std::string& levelName, const sf::Vector2i& levelSize, int tileSize)
	: m_tileLayers(std::move(tileLayers)),
	m_collisionLayer(std::move(collisionLayer)),
	m_levelName(levelName),
	m_levelSize(levelSize),
	m_tileSize(tileSize)
{
}

void Level::draw(sf::RenderWindow& window)
{
	for (auto& tileLayer : m_tileLayers)
	{
		tileLayer.draw(window);
	}
}

bool Level::collisionTileAtPosition(const sf::Vector2i & position) const
{
	for (const auto& collidableTile : m_collisionLayer)
	{
		const sf::Vector2i tilePosition(std::floor(collidableTile.x / m_tileSize), std::floor(collidableTile.y / m_tileSize));
		if (position == tilePosition)
		{
			return true;
		}
	}
	return false;
}

const std::vector<sf::Vector2i>& Level::getCollisionLayer() const
{
	return m_collisionLayer;
}

int Level::getTileSize() const
{
	return m_tileSize;
}