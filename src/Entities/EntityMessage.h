#pragma once

#include <Entities\EntityTag.h>
#include <Entities\EntityEvent.h>

class EntityMessage
{
public:
	EntityMessage(EntityEvent event, int charges);
	
	bool hasCharges() const;
	void decreaseCharge();

	const EntityEvent m_entityEvent;
private:
	int m_charges;
};