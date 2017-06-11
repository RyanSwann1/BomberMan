#include "TileLayer.h"
#include <assert.h>
TileLayer::TileLayer(const std::vector<std::vector<int>>& tileMap, const sf::Vector2i& mapSize)
	: m_tileMap()
{
	for (int row = 0; row < mapSize.y; ++row)
	{
		for (int col = 0; col < mapSize.x; ++col)
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

void TileLayer::draw(sf::RenderWindow& window)
{
	for (auto& tile : m_tileMap)
	{
		tile->draw(window);
	}
}