#include <Entities\EntityMessage.h>

EntityMessage::EntityMessage(EntityEvent event, int charges)
	: m_entityEvent(event),
	m_charges(charges)
{
}

bool EntityMessage::hasCharges() const
{
	return (m_charges > 0 ? true : false);
}

void EntityMessage::decreaseCharge()
{
	--m_charges;
}
