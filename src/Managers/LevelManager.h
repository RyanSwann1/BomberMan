#pragma once

#include <Utilities\FileDirectory.h>
#include <Level\Level.h>

class LevelManager 
{
public:
	LevelManager();

	void draw(sf::RenderWindow& window);

private:
	std::vector<Level> m_levels;
	std::vector<FileDirectory> m_levelDirectories;
	Level* m_currentLevel;
	int m_currentLevelIndex;

	void loadNextLevel();
};