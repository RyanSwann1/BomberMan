#include "EventManager.h"
#include <Managers\StateManager.h>
#include <Locators\EventManagerLocator.h>
#include <assert.h>

EventManager::EventManager(const StateManager & stateManager)
	: m_stateManager(stateManager),
	m_callBacks(),
	m_bindings()
{
	EventManagerLocator::provide(*this);
}

void EventManager::update(sf::Event currentEvent)
{
	for (auto& binding : m_bindings)
	{
		for (auto& event : binding.m_events)
		{
			if (currentEvent.KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(event.m_keybind)))
				{
					m_callBacks.push_back(binding.m_callBack);
				}
			}
		}
	}
}

void EventManager::handleCallbacks()
{
	for (auto& callBack : m_callBacks)
	{
		callBack();
	}

	m_callBacks.clear();
}

void EventManager::registerBinding(const std::function<void()>& callBack, std::string&& name, StateType owningState, Keybind keybind)
{
	auto iter = std::find_if(m_bindings.cbegin(), m_bindings.cend(), [&name](const auto& binding) { return binding.m_name == name; });
	assert(iter == m_bindings.cend());

	m_bindings.emplace_back(callBack, std::move(name), owningState, keybind);
}

void EventManager::unregisterBinding(const std::string & name, Keybind keybind)
{
	//auto iter = std::find_if(m_bindings.begin(), m_bindings.end(), [&name](const auto& binding) { return binding.m_name == name; });
	//assert(iter != m_bindings.cend());
	//m_bindings.erase(iter);
}

void EventManager::unregisterKeybinding(const std::string & bindingName, Keybind keybind)
{	
	
}

bool EventManager::bindingNameFound(const std::string & bindingName) const
{
	auto cIter = std::find_if(m_bindings.cbegin(), m_bindings.cend(),
		[&bindingName](const auto& binding) { return binding.m_name == bindingName; });

	return (cIter != m_bindings.cend() ? true : false);
}

void EventManager::activateCallback()
{
	for (auto& callBack : m_callBacks)
	{
		callBack();
	}

	m_callBacks.clear();
}