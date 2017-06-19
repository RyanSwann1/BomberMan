#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <assert.h>
#include <utility>
#include <algorithm>

template <class MessageType>
class MessageHandler
{
	class Listener
	{
	public:
		Listener(const std::function<void()>& fp, std::string&& listenerName)
			: m_listeners(),
			m_name(std::move(listenerName))
		{
			m_listeners.push_back(fp);
		}

		std::vector<std::function<void()>> m_listeners;
		const std::string m_name;
	};

public:
	void subscribe(const std::function<void()>& fp, std::string&& listenerName, MessageType message)
	{
		auto iter = m_listeners.find(message);
		if (iter != m_listeners.cend())
		{
			iter->second.m_listeners.push_back(fp);
		}
		else
		{
			m_listeners.emplace(message, Listener(fp, std::move(listenerName)));
		}
	}

	void broadcast(MessageType message)
	{
		auto iter = m_listeners.find(message);
		assert(iter != m_listeners.cend());
		for (auto& listener : iter->second.m_listeners)
		{
			listener();
		}
	}

	void unsubscribe(MessageType message, const std::string& listenerName)
	{
		auto iter = m_listeners.find(message);
		assert(iter != m_listeners.cend());
		auto listener = std::find_if(iter->second.m_listeners.begin(), iter->second.m_listeners.end(), [&listenerName](const auto& listener) {return listener.m_name == listenerName; });
		assert(listener != iter->second.m_listeners.cend());
		iter->second.m_listeners.erase(listener);
	}

private:
	std::unordered_map<MessageType, Listener> m_listeners;
};