#include "LevelManager.h"
#include <Level\LevelParser.h>
#include <Locators\TileSheetManagerLocator.h>
#include <Locators\LevelManagerLocator.h>
#include <Utilities\Utilities.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Locators\AudioPlayerLocator.h>
#include <Audio\AudioPlayer.h>
#include <assert.h>
#include <utility>
#include <algorithm>
#include <iostream>

LevelManager::LevelManager(EntityManager& entityManager)
	: m_entityManager(entityManager),
	m_level(),
	m_levelDirectories(Utilities::getFileDirectories(Utilities::getResourceDirectory() + "Maps.txt")),
	m_currentLevelIndex(0),
	m_loadNextLevel(false),
	m_reloadCurrentLevel(false)
{
	LevelManagerLocator::provide(*this);
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.subscribe(std::bind(&LevelManager::loadNextLevel, this), "LevelManager", GameEvent::ChangeToNextLevel);
	gameEventMessenger.subscribe(std::bind(&LevelManager::parseLevel, this), "LevelManager", GameEvent::ReloadCurrentLevel);

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
		AudioPlayerLocator::getAudioClipPlayer().playMusic("LevelMusic.wav");
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
	m_level = LevelParser::parseLevel(fileDirectory, fileName, m_entityManager);
	std::cout << "Level Parsed\n";
}