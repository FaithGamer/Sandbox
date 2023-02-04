#include "pch.h"
#include "Timing.h"


namespace Sandbox
{

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