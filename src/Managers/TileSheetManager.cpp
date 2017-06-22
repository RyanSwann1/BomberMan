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

const TileSheet & TileSheetManager::getTileSheet(const std::string & tileSheetName) const
{
	auto iter = std::find_if(m_tileSheets.cbegin(), m_tileSheets.cend(), [&tileSheetName](const auto& tileSheet) {return tileSheet.second.m_name == tileSheetName; });
	assert(iter != m_tileSheets.cend());
	return iter->second;
}

const TileSheet& TileSheetManager::getTileSheet(int tileID) const
{
	const TileSheet* tileSheet = nullptr;
	//Get appropriate tilesheet with requested tileID
	for (auto iter = m_tileSheets.cbegin(); iter != m_tileSheets.cend();)
	{
		if (tileID == 0)
		{
			tileSheet = &m_tileSheets.begin()->second;
			break;
		}

		const int firstGID = iter->second.m_firstGID;
		const int tile = (tileID - firstGID);
		if (tile >= 0)
		{
			tileSheet = &iter->second;
		}
		if (tileID == 0)
		{
			int i = 0;
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
