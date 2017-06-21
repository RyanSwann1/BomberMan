#include "LevelManager.h"
#include <Level\LevelParser.h>
#include <Locators\TileSheetManagerLocator.h>
#include <Locators\LevelManagerLocator.h>
#include <Utilities\Utilities.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <assert.h>
#include <utility>
#include <algorithm>

LevelManager::LevelManager()
	: m_level(),
	m_levelDirectories(Utilities::getFileDirectories("res/Maps.txt")),
	m_currentLevelIndex(0),
	m_loadNextLevel(false),
	m_reloadCurrentLevel(false)
{
	LevelManagerLocator::provide(*this);
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&LevelManager::setToLoadNextLevel, this), "LevelManager", GameEvent::ChangeToNextLevel);
	gameEventMessenger.subscribe(std::bind(&LevelManager::setToReloadCurrentLevel, this), "LevelManager", GameEvent::ReloadCurrentLevel);

	loadNextLevel();
}

LevelManager::~LevelManager()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("LevelManager", GameEvent::ChangeToNextLevel);
	gameEventMessenger.unsubscribe("LevelManager", GameEvent::ReloadCurrentLevel);
}

void LevelManager::draw(sf::RenderWindow& window)
{
	assert(m_level.get());
	m_level->draw(window);
}

void LevelManager::update()
{
	if (m_loadNextLevel)
	{
		loadNextLevel();
		m_loadNextLevel = false;
	}
	else if (m_reloadCurrentLevel)
	{
		parseLevel();
		m_reloadCurrentLevel = false;
	}
}

const std::unique_ptr<Level>& LevelManager::getCurrentLevel() const
{
	assert(m_level.get());
	return m_level;
}

void LevelManager::loadNextLevel()
{
	if (m_currentLevelIndex < m_levelDirectories.size())
	{
		++m_currentLevelIndex;
		parseLevel();
	}
	else
	{
		GameEventMessengerLocator::getGameEventMessenger().broadcast(GameEvent::WinGame);
	}
}

void LevelManager::parseLevel()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.broadcast(GameEvent::ClearMap);
	const auto& fileDirectory = m_levelDirectories.at(m_currentLevelIndex - 1).m_fileDirectory;
	const auto& fileName = m_levelDirectories.at(m_currentLevelIndex - 1).m_fileName;
	m_level = LevelParser::parseLevel(fileDirectory, fileName, EntityManagerLocator::getEntityManager());
}

void LevelManager::setToLoadNextLevel()
{
	m_loadNextLevel = true;
}

void LevelManager::setToReloadCurrentLevel()
{
	m_reloadCurrentLevel = true;
}