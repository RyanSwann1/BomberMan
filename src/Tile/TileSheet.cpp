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
	////Calculate row position
	//const int y = tileID - (m_rows * m_columns);

	//int row = 0;

	////Calculate column position
	//const int column = tileID - (row * m_columns);

	//sf::IntRect x((m_margin + (m_spacing + m_tileSize) * column),
	//	(m_margin + (m_spacing + m_tileSize) * row),
	//	m_tileSize, m_tileSize);

	//return sf::IntRect((m_margin + (m_spacing + m_tileSize) * column),
	//	(m_margin + (m_spacing + m_tileSize) * row),
	//	m_tileSize, m_tileSize);


	//Scan through every tile on tilesheet to find appropriate tile by ID
	int column = 0, row = 0;
	const int lastTileID = m_columns * m_rows;
	for (int i = 0; i < lastTileID; i++)
	{
		//If matching tile found
		if (tileID == i)
		{
			break;
		}

		++column;
		
		//Move onto the next row
		if (column == m_columns)
		{
			column = 0;
			++row;
		}
	}

	assert(tileID <= lastTileID);

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