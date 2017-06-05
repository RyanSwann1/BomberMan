#pragma once

#include <Tile\TileSheet.h>
#include <string>
#include <map>

class TileSheetManager
{
public:
	TileSheetManager();
		
	const TileSheet& getTileSheet(int tileID) const;
	bool hasTileSheet(int firstGID) const;
	void addTileSheet(int firstGID, const TileSheet& tileSheet);
	
private:
	std::map<int, TileSheet> m_tileSheets;
};