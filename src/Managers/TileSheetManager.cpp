#include "TileSheetManager.h"
#include <assert.h>
#include <Locators\TileSheetManagerLocator.h>
#include <utility>

TileSheetManager::TileSheetManager()
{
	TileSheetManagerLocator::provide(*this);
}

void TileSheetManager::addTileSheet(int firstGID, const TileSheet & tileSheet)
{
	m_tileSheets.insert(std::pair<int, TileSheet>(firstGID, tileSheet));
}

const TileSheet& TileSheetManager::getTileSheet(int tileID) const
{
	const TileSheet* tileSheet = nullptr;
	//Get appropriate tilesheet with requested tileID
	for (auto iter = m_tileSheets.cbegin(); iter != m_tileSheets.cend();)
	{
		const int firstGID = iter->second.m_firstGID;
		const int tile = (tileID - firstGID);
		if (tile <= 0)
		{
			--iter;
			tileSheet = &iter->second;
			break;
		}
		++iter;
	}

	assert(tileSheet);
	return *tileSheet;
}

bool TileSheetManager::hasTileSheet(int firstGID) const
{
	auto iter = m_tileSheets.find(firstGID);
	return(iter != m_tileSheets.cend() ? true : false);
}
