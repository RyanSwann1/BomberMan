#pragma once

#include <Tile\Tile.h>
#include <vector>
#include <memory>

class TileLayer
{
public:
	TileLayer(const std::vector<std::vector<int>>& tileMap, const sf::Vector2i& mapSize);

	void draw(sf::RenderWindow& window);

private:
	std::vector<std::unique_ptr<Tile>> m_tileMap;
};