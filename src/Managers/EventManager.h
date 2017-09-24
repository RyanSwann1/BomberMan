#pragma once

#include <States\StateType.h>
#include <functional>
#include <vector>
#include <SFML\window.hpp>
#include <utility>

enum class Keybind
{
	A = sf::Keyboard::A,
	D = sf::Keyboard::D,
	W = sf::Keyboard::W,
	S = sf::Keyboard::S,
	SPACE = sf::Keyboard::Space
};

class Binding
{
	class Event
	{
	public:
		Event(Keybind keyBind)
			: m_keybind(keyBind)
		{}

		const Keybind m_keybind;
	};

public:
	Binding(const std::function<void()>& callback, std::string&& name, StateType stateType, Keybind keybind)
		: m_name(std::move(name)),
		m_callBack(callback),
		m_stateType(stateType),
		m_events()
	{
		m_events.emplace_back(keybind);
	}

	const std::string m_name;
	const std::function<void()> m_callBack;
	const StateType m_stateType;
	std::vector<Event> m_events;
	int m_counter;
};

class StateManager;
class EventManager
{
public:	
	EventManager(const StateManager& stateManager);

	void registerBinding(const std::function<void()>& callBack, std::string&& bindingName, StateType owningState, Keybind keybind);
	void unregisterBinding(const std::string& name, Keybind keybind);
	void unregisterKeybinding(const std::string& bindingName, Keybind keybind);

	void update(sf::Event currentEvent);
	void handleCallbacks();

private:
	const StateManager& m_stateManager;
	std::vector<std::function<void()>> m_callBacks;
	std::vector<Binding> m_bindings;

	bool bindingNameFound(const std::string& bindingName) const;
	void activateCallback();
};