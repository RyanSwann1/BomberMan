#include "Tile.h"
#include <Locators\TileSheetManagerLocator.h>
#include <Managers\TileSheetManager.h>
#include <Window.h>

Tile::Tile(const sf::Vector2i & position, int tileID)
	: m_ID(tileID), 
	m_sprite(),
	m_tileSheet(TileSheetManagerLocator::getTileSheetManager().getTileSheet(tileID))
{
	m_sprite.setTexture(m_tileSheet.getTexture());
	m_sprite.setTextureRect(m_tileSheet.getTileLocation(tileID));
	m_sprite.setPosition(sf::Vector2f(position.x * m_tileSheet.m_tileSize, position.y * m_tileSheet.m_tileSize));
}

Tile::Tile(const Tile& orig)
	: m_ID(orig.m_ID),
	m_tileSheet(orig.m_tileSheet)
{
	m_sprite.setTexture(m_tileSheet.getTexture());
	m_sprite.setTextureRect(m_tileSheet.getTileLocation(m_ID));
	m_sprite.setPosition(orig.m_sprite.getPosition());
}

Tile::~Tile()
{
	m_tileSheet.releaseTileSheet();
}

void Tile::draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}