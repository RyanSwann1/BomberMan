#pragma once


class Timer
{
public:
	Timer(float expirationTime, bool active = true);
	
	void update(float deltaTime);
	bool isExpired() const;
	void reset();

private:
	const float m_expirationTime;
	float m_elaspedTime;
	bool m_active;
};