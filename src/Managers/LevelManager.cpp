#include "LevelManager.h"
#include <Level\LevelParser.h>
#include <Locators\TileSheetManagerLocator.h>
#include <Locators\LevelManagerLocator.h>
#include <Utilities\Utilities.h>
#include <assert.h>
#include <utility>
#include <algorithm>

LevelManager::LevelManager()
	: m_levels(),
	m_levelDirectories(Utilities::getFileDirectories("res/Maps.txt")),
	m_currentLevel(nullptr),
	m_currentLevelIndex(0)
{
	LevelManagerLocator::provide(*this);
	loadNextLevel();
}

void LevelManager::draw(sf::RenderWindow& window)
{
	m_currentLevel->draw(window);
}

const Level & LevelManager::getCurrentLevel() const
{
	assert(m_currentLevel);
	return *m_currentLevel;
}

void LevelManager::loadNextLevel()
{
	if (m_currentLevelIndex < m_levelDirectories.size())
	{
		const auto& fileDirectory = m_levelDirectories.at(m_currentLevelIndex).m_fileDirectory;
		const auto& fileName = m_levelDirectories.at(m_currentLevelIndex).m_fileName;
		m_levels.emplace_back(LevelParser::parseLevel(fileDirectory, fileName, EntityManagerLocator::getEntityManager()));
		//TODO:: Change how current level is found
		m_currentLevel = m_levels.back().get();
		++m_currentLevelIndex;
	}
}