#pragma once

#include <assert.h>

class EntityMessenger;
class EntityMessengerLocator
{
public:
	static void provide(EntityMessenger& entityMessageHandler)
	{
		m_entityMessageHandler = &entityMessageHandler;
	}

	static EntityMessenger& getEntityMessenger()
	{
		assert(m_entityMessageHandler);
		return *m_entityMessageHandler;
	}

private:
	static EntityMessenger* m_entityMessageHandler;
};