#pragma once

#include <Utilities\FileDirectory.h>
#include <Level\Level.h>
#include <memory>

class EntityManager;
class LevelManager 
{
public:
	LevelManager(EntityManager& entityManager);
	~LevelManager();

	const std::unique_ptr<Level>& getCurrentLevel() const;
	void draw(sf::RenderWindow& window);

private:
	EntityManager& m_entityManager;
	std::unique_ptr<Level> m_level;
	std::vector<FileDirectory> m_levelDirectories;
	int m_currentLevelIndex;
	bool m_loadNextLevel;
	bool m_reloadCurrentLevel;

	void loadNextLevel();
	void parseLevel();
};