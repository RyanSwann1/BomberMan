#pragma once
#include <assert.h>

class CollidableBoxSpawner;
class CollidableBoxSpawnerLocator
{
public:
	static void provide(CollidableBoxSpawner& collidableBoxSpawner)
	{
		m_collidableBoxSpawner = &collidableBoxSpawner;
	}

	static CollidableBoxSpawner& getCollidableBoxSpawner()
	{
		assert(m_collidableBoxSpawner);
		return *m_collidableBoxSpawner;
	}

private:
	static CollidableBoxSpawner* m_collidableBoxSpawner;
};