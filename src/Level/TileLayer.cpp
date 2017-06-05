#include "TileLayer.h"
#include "LevelDetails.h"
#include <assert.h>

TileLayer::TileLayer(const std::vector<std::vector<int>>& tileMap, const LevelDetails & levelDetails)
	: m_tileMap()
{
	for (int row = 0; row < levelDetails.m_size.y; ++row)
	{
		for (int col = 0; col < levelDetails.m_size.x; ++col)
		{
			const int tileID = tileMap[row][col];
			if (tileID >= 0)
			{
				m_tileMap.emplace_back(std::make_unique<Tile>(sf::Vector2i(col, row), tileID));
			}
		}
	}
	assert(!m_tileMap.empty());
}

TileLayer::TileLayer(TileLayer & const orig)
	: m_tileMap(std::move(orig.m_tileMap))
{
}

void TileLayer::draw(sf::RenderWindow& window)
{
	for (auto& tile : m_tileMap)
	{
		tile->draw(window);
	}
}