#pragma once

#include <Utilities\FileDirectory.h>
#include <Locators\EntityManagerLocator.h>
#include <Level\Level.h>
#include <memory>

class LevelManager : private EntityManagerLocator
{
public:
	LevelManager();
	~LevelManager();

	const std::unique_ptr<Level>& getCurrentLevel() const;
	void draw(sf::RenderWindow& window);
	void update();

private:
	std::unique_ptr<Level> m_level;
	std::vector<FileDirectory> m_levelDirectories;
	int m_currentLevelIndex;
	bool m_loadNextLevel;
	bool m_reloadCurrentLevel;

	void loadNextLevel();
	void parseLevel();

	void setToLoadNextLevel();
	void setToReloadCurrentLevel();
};