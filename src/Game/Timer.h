#pragma once


class Timer
{
public:
	Timer(float expirationTime, bool active);
	
	float getElaspedTime() const;
	void update(float deltaTime);
	bool isActive() const;
	bool isExpired() const;
	void reset();
	void activate();
	void deactivate();

private:
	const float m_expirationTime;
	float m_elaspedTime;
	bool m_active;
};