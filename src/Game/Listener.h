#pragma once

#include <functional>
#include <string>

template <class MessageType>
class Listener
{
public:
	Listener(const std::function<void(MessageType&)>& fp, std::string&& listenerName)
		: m_listener(fp),
		m_name(std::move(listenerName))
	{}

	std::function<void(MessageType&)> m_listener;
	std::string m_name;
};