#pragma once

#include <Tile\Tile.h>
#include <vector>
#include <memory>

class LevelDetails;
class TileLayer
{
public:
	TileLayer(const std::vector<std::vector<int>>& tileMap, const LevelDetails& levelDetails);

	void draw(sf::RenderWindow& window);

private:
	std::vector<std::unique_ptr<Tile>> m_tileMap;
};