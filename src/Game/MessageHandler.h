#pragma once

#include <unordered_map>
#include <vector>
#include <assert.h>
#include <utility>
#include <algorithm>
#include <Game\Listener.h>

template <class MessageType>
class MessageHandler
{
public:
	void subscribe(const std::function<void(MessageType)>& fp, std::string&& listenerName, MessageType message)
	{
		auto iter = m_listeners.find(message);
		if (iter != m_listeners.cend())
		{
			iter->second.emplace_back(fp, std::move(listenerName));
		}
		else
		{
			m_listeners.emplace(message, std::vector<Listener<MessageType>>{Listener<MessageType>(fp, std::move(listenerName))});
		}
	}

	/*virtual void subscribe(const std::function<void(MessageType&)>& fp, std::string&& listenerName, const MessageType& message) = 0;
	virtual void broadcast(MessageType& message) = 0;
	virtual void unsubscribe(const std::string& listenerName, const MessageType& message) = 0;*/

	virtual void broadcast(MessageType message)
	{
		auto iter = m_listeners.find(message);
		assert(iter != m_listeners.cend());
		
		for (const auto& listener : iter->second)
		{
			listener.m_listener(message);
		}
	}

	void unsubscribe(const std::string& listenerName, MessageType message)
	{
		auto iter = m_listeners.find(message);
		assert(iter != m_listeners.cend());
		
		auto listener = std::find_if(iter->second.begin(), iter->second.end(), 
			[&listenerName](const auto& listener) { return listener.m_name == listenerName; });
		assert(listener != iter->second.cend());
		iter->second.erase(listener);
	}

protected:
	std::unordered_map<MessageType, std::vector<Listener<MessageType>>> m_listeners;
};