#pragma once

#include <assert.h>
class LevelManager;
class LevelManagerLocator
{
public:
	static void provide(LevelManager& levelManager)
	{
		m_levelManager = &levelManager;
	}


	static const LevelManager& getLevelManager()
	{
		return *m_levelManager;
	}

private:
	static LevelManager* m_levelManager;
};