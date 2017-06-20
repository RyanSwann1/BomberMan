#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <assert.h>
#include <utility>
#include <algorithm>
#include <memory>

template <class MessageType>
class MessageHandler
{
	class Listener
	{
	public:
		Listener(const std::function<void()>& fp, std::string&& listenerName)
			: m_listener(fp),
			m_name(std::move(listenerName))
		{}

		std::function<void()> m_listener;
		std::string m_name;
	};

public:
	void subscribe(const std::function<void()>& fp, std::string&& listenerName, MessageType message)
	{
		auto iter = m_listeners.find(message);
		if (iter != m_listeners.cend())
		{
			iter->second.emplace_back(fp, std::move(listenerName));
		}
		else
		{
			m_listeners.emplace(message, std::vector<Listener>{Listener(fp, std::move(listenerName))});
		}
	}

	void broadcast(MessageType message)
	{
		auto iter = m_listeners.find(message);
		assert(iter != m_listeners.cend());
		for (const auto& listener : iter->second)
		{
			listener.m_listener();
		}
	}

	void unsubscribe(const std::string& listenerName, MessageType message)
	{
		auto iter = m_listeners.find(message);
		assert(iter != m_listeners.cend());
		auto listener = std::find_if(iter->second.begin(), iter->second.end(), 
			[&listenerName](const auto& listener) {return listener.m_name == listenerName; });
		assert(listener != iter->second.cend());
		iter->second.erase(listener);
	}

private:
	std::unordered_map<MessageType, std::vector<Listener>> m_listeners;
};