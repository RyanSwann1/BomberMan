#pragma once

#include <Game\GameEvent.h>
#include <assert.h>

template <class GameEvent>
class MessageHandler;
class GameEventMessengerLocator
{
public:
	static void provide(MessageHandler<GameEvent>& gameEventMessenger)
	{
		m_gameEventMessenger = &gameEventMessenger;
	}

	static MessageHandler<GameEvent>& getGameEventMessenger()
	{
		assert(m_gameEventMessenger);
		return *m_gameEventMessenger;
	}

private:
	static MessageHandler<GameEvent>* m_gameEventMessenger;
};