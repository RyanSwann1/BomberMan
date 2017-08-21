#pragma once


class Timer
{
public:
	Timer(float expirationTime, bool active);
	
	float getExpirationTime() const;
	float getElaspedTime() const;
	bool isActive() const;
	bool isExpired() const;
	void reduceExpirationTime(float i);
	void update(float deltaTime);
	void reset();
	void activate();
	void deactivate();

private:
	float m_expirationTime;
	float m_elaspedTime;
	bool m_active;
};