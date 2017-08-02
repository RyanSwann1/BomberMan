#include <Game\Timer.h>

Timer::Timer(float expirationTime, bool active)
	: m_expirationTime(expirationTime),
	m_elaspedTime(0),
	m_active(active)
{
}

float Timer::getExpirationTime() const
{
	return m_expirationTime;
}

float Timer::getElaspedTime() const
{
	return m_elaspedTime;
}

void Timer::update(float deltaTime)
{
	if (!m_active)
	{
		return;
	}

	m_elaspedTime += deltaTime;
}

bool Timer::isActive() const
{
	return m_active;
}

bool Timer::isExpired() const
{
	return m_elaspedTime >= m_expirationTime;
}

void Timer::reset()
{
	m_elaspedTime = 0;
}

void Timer::activate()
{
	m_active = true;
}

void Timer::deactivate()
{
	m_active = false;
}
