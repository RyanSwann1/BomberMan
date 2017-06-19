#pragma once

#include <deque>

template <class EventType>
class EventQueue
{
public:
	bool isEmpty() const
	{
		return m_eventQueue.empty();
	}

	EventType getNextEvent()
	{
		const auto event = m_eventQueue.front();
		m_eventQueue.pop_front();
		return event;
	}

	void addEvent(EventType eventType)
	{
		m_eventQueue.push_back(eventType);
	}

private:
	std::deque<EventType> m_eventQueue;
};
