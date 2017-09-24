#pragma once

#include <assert.h>

class EventManager;
class EventManagerLocator
{
public:
	static void provide(EventManager& eventManager)
	{
		m_eventManager = &eventManager;
	}

	static EventManager& getEventManager()
	{
		assert(m_eventManager);
		return *m_eventManager;
	}

private:
	static EventManager* m_eventManager;
};