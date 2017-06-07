#pragma once

#include <Utilities\FileDirectory.h>
#include <Locators\EntityManagerLocator.h>
#include <Level\Level.h>
#include <memory>

class LevelManager : private EntityManagerLocator
{
public:
	LevelManager();

	void draw(sf::RenderWindow& window);

private:
	std::vector<std::unique_ptr<Level>> m_levels;
	std::vector<FileDirectory> m_levelDirectories;
	Level* m_currentLevel;
	int m_currentLevelIndex;

	void loadNextLevel();
};