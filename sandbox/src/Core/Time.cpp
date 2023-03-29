#include "pch.h"
#include "Core/Time.h"


namespace Sandbox
{
	Time Time::operator+(const Time& other)
	{
		Time time = *this;
		time.m_microseconds += other.m_microseconds;
		return time;
	}
	Time& Time::operator+=(const Time& other)
	{
		m_microseconds += other.m_microseconds;
		return *this;
	}

	////////////////////////////
	/// Clock //////////////////
	////////////////////////////

	Clock::Clock() :
		m_start(std::chrono::high_resolution_clock::now())
	{

	}

	Time Clock::GetElapsed() const
	{
		return Time(std::chrono::duration_cast<std::chrono::microseconds>((std::chrono::high_resolution_clock::now() - m_start)));
	}

	Time Clock::Restart()
	{
		Time t = GetElapsed();
		m_start = std::chrono::high_resolution_clock::now();
		return t;
	}
}