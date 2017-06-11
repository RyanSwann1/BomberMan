#pragma once

#include <SFML\Graphics.hpp>
class TileSheet;
class Tile
{
public:
	Tile(const sf::Vector2i& position, int tileID);
	Tile(const Tile& orig);
	~Tile();

	void draw(sf::RenderWindow& window);

private:
	const int m_ID;
	const TileSheet& m_tileSheet;
	sf::Sprite m_sprite;
};