#include "EntityMessenger.h"
#include <Managers\EntityManager.h>
#include <Entities\EntityMessage.h>
#include <Locators\EntityMessengerLocator.h>

EntityMessenger::EntityMessenger()
	: m_listeners()
{
	EntityMessengerLocator::provide(*this);
}

void EntityMessenger::subscribe(const std::function<void(EntityMessage&)>& fp, std::string && listenerName, EntityEvent entityEvent)
{
	auto iter = m_listeners.find(entityEvent);
	if (iter != m_listeners.cend())
	{
		iter->second.emplace_back(fp, std::move(listenerName));
	}
	else
	{
		m_listeners.emplace(entityEvent, std::vector<Listener<EntityMessage>>{Listener<EntityMessage>(fp, std::move(listenerName))});
	}
}

void EntityMessenger::unsubscribe(const std::string & listenerName, EntityEvent entityEvent)
{
	auto iter = m_listeners.find(entityEvent);
	assert(iter != m_listeners.cend());

	for (auto i = iter->second.begin(); i != iter->second.end(); ++i)
	{
		if (i->m_name == listenerName)
		{
			iter->second.erase(i);
			break;
		}
	}
}

void EntityMessenger::broadcast(EntityMessage& entityMessage)
{
	auto iter = m_listeners.find(entityMessage.m_entityEvent);
	assert(iter != m_listeners.cend());

	for (auto& listener : iter->second)
	{
		if (!entityMessage.hasCharges())
		{
			break;
		}

		listener.m_listener(entityMessage);
	}
}
