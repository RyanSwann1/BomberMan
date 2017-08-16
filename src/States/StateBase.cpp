#include <States\StateBase.h>

StateBase::StateBase(StateManager & stateManager, StateType stateType)
	: m_stateManager(stateManager),
	m_gui(*this),
	m_stateType(stateType)
{
}

StateBase::~StateBase()
{
}

StateType StateBase::getType()
{
	return m_stateType;
}

void StateBase::update(float deltaTime)
{
	m_gui.update(deltaTime);
}

void StateBase::draw(sf::RenderWindow & window)
{
	m_gui.draw(window);
}