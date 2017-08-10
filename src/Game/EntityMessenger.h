#pragma once

#include <unordered_map>
#include <string>
#include <Game\Listener.h>
#include <Entities\EntityMessage.h>

class EntityMessenger
{
public:
	EntityMessenger();

	void subscribe(const std::function<void(EntityMessage&)>& fp, std::string&& listenerName, EntityEvent entityEvent);
	void unsubscribe(const std::string& listenerName, EntityEvent entityEvent);
	void broadcast(EntityMessage& entityMessage);

private:
	std::unordered_map<EntityEvent, std::vector<Listener<EntityMessage>>> m_listeners;
};


//class EntityMessageHandler : public MessageHandler<EntityEvent>, private EntityManagerLocator
//{
//public:
//	//void subscribe(const std::function<EntityMessage&>& fp, std::string&& listenerName, EntityEvent entityEvent) override;
//	////void subscribe(const std::function<void(EntityMessage&)>& fp, std::string&& listenerName, const EntityMessage& message) override;
//	//void broadcast(EntityMessage& message) override;
//
//private:
//	std::unordered_map<EntityEvent, std::vector<Listener<EntityMessage>>> m_listeners;
//};