#include "TileSheet.h"
#include <Managers\TextureManager.h>
#include <utility>

TileSheet::TileSheet(std::string&& tileSheetName, int tileSize, int columns, int rows, int firstGID, int margin, int spacing)
	: m_name(std::move(tileSheetName)),
	m_tileSize(tileSize),
	m_columns(columns),
	m_rows(rows),
	m_firstGID(firstGID),
	m_margin(margin),
	m_spacing(spacing)
{}

sf::IntRect TileSheet::getTileLocation(int tileID) const
{
	const int row = tileID / m_columns;
	const int column = tileID % m_columns;

	//Kept here for profiling 

	////Scan through every tile on tilesheet to find appropriate tile by ID
	//int column = 0, row = 0;
	//const int lastTileID = m_columns * m_rows;
	//for (int i = 0; i < lastTileID; i++)
	//{
	//	//If matching tile found
	//	if (tileID == i)
	//	{
	//		break;
	//	}
	//	++column;
	//	
	//	//Move onto the next row
	//	if (column == m_columns)
	//	{
	//		column = 0;
	//		++row;
	//	}
	//}

	return sf::IntRect((m_margin + (m_spacing + m_tileSize) * column),
		(m_margin + (m_spacing + m_tileSize) * row),
		m_tileSize, m_tileSize);
}


const sf::Texture & TileSheet::getTexture() const
{
	return TextureManagerLocator::getTextureManager().getResource(m_name);
}

void TileSheet::releaseTileSheet() const
{
	TextureManagerLocator::getTextureManager().releaseResource(m_name);
}