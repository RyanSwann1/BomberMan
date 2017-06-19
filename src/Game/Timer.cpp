#include <Game\Timer.h>

Timer::Timer(float expirationTime, bool active)
	: m_expirationTime(expirationTime),
	m_elaspedTime(0),
	m_active(active)
{
}

void Timer::update(float deltaTime)
{
	if (!m_active)
	{
		return;
	}

	m_elaspedTime += deltaTime;
}

bool Timer::isExpired() const
{
	return m_elaspedTime >= m_expirationTime;
}

void Timer::reset()
{
	m_elaspedTime = 0;
}
