#pragma once
#include "LevelDetails.h"
#include "CollisionLayer.h"
#include "TileLayer.h"

class Level
{
public:
	Level(std::vector<TileLayer>&& tileLayers, const CollisionLayer& collisionLayer, const LevelDetails& levelDetails, const std::string& levelName);
	void draw(sf::RenderWindow& window);

private:
	std::vector<TileLayer> m_tileLayers;
	const CollisionLayer m_collisionLayer;
	const LevelDetails m_levelDetails;
	const std::string m_levelName;
};