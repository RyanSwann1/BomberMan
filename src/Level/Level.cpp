#include "Level.h"
#include <utility>

Level::Level(std::vector<TileLayer>&& tileLayers, const CollisionLayer & collisionLayer, const LevelDetails & levelDetails, const std::string& levelName)
	: m_tileLayers(std::move(tileLayers)),
	m_collisionLayer(collisionLayer),
	m_levelDetails(levelDetails),
	m_levelName(levelName)
{
}

void Level::draw(sf::RenderWindow& window)
{
	for (auto& tileLayer : m_tileLayers)
	{
		tileLayer.draw(window);
	}
}