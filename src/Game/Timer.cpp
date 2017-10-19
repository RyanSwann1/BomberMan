#include <Game\Timer.h>

Timer::Timer(float expirationTime, bool active, bool reversalTimer, float startingTime)
	: m_reversalTimer(reversalTimer),
	m_startingTime(startingTime),
	m_expirationTime(expirationTime),
	m_elaspedTime(startingTime),
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

	(m_reversalTimer ? m_elaspedTime -= deltaTime : m_elaspedTime += deltaTime);
}

bool Timer::isActive() const
{
	return m_active;
}

bool Timer::isExpired() const
{
	return (m_reversalTimer ? m_elaspedTime <= m_expirationTime : m_elaspedTime >= m_expirationTime);
}

void Timer::reduceExpirationTime(float i)
{
	if (m_expirationTime - i > 0.0f)
	{
		m_expirationTime -= i;
	}
}

void Timer::reset()
{
	m_elaspedTime = m_startingTime;
}

void Timer::activate()
{
	m_active = true;
}

void Timer::deactivate()
{
	m_active = false;
}