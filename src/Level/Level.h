#pragma once
#include "TileLayer.h"

class Level
{
public:
	Level(std::vector<TileLayer>&& tileLayers, const std::string& levelName, const sf::Vector2i& levelSize, int tileSize);
	
	const sf::Vector2i& getSize() const;
	const std::string& getName() const;
	//const std::vector<sf::Vector2i>& getCollisionLayer() const;
	int getTileSize() const;
	
	void draw(sf::RenderWindow& window);

private:
	std::vector<TileLayer> m_tileLayers;
	//const std::vector<sf::Vector2i> m_collisionLayer;
	const std::string m_levelName;
	const sf::Vector2i m_levelSize;
	const int m_tileSize;
};