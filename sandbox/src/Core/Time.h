#pragma once
#include <chrono>

namespace Sandbox
{
	/// <summary>
	/// Wrapper around a std::chrono::microseconds to represent time duration
	/// Can be implicitly converted to float to represent seconds
	/// </summary>
	class Time
	{
	public:
		Time();

		Time(const float seconds);

		template <typename rep, typename ratio>
		Time(const std::chrono::duration<rep, ratio>& duration);

		template <typename chrono_duration = std::chrono::microseconds>
		chrono_duration GetDuration() const;

		operator float() const;

	private:

		std::chrono::microseconds m_microseconds;

		friend Time& operator+=(Time& l, const Time& r);
		friend Time& operator-=(Time& l, const Time& r);
		friend Time& operator*=(Time& l, const Time& r);
		friend Time& operator/=(Time& l, const Time& r);
		friend Time& operator%=(Time& l, const Time& r);

		friend Time operator+(const Time& l, const Time& r);
		friend Time operator-(const Time& l, const Time& r);
		friend Time operator*(const Time& l, const Time& r);
		friend Time operator/(const Time& l, const Time& r);
		friend Time operator%(const Time& l, const Time& r);
	};

	Time& operator+=(Time& l, const Time& r);
	Time& operator-=(Time& l, const Time& r);
	Time& operator*=(Time& l, const Time& r);
	Time& operator/=(Time& l, const Time& r);
	Time& operator%=(Time& l, const Time& r);

	Time operator+(const Time& l, const Time& r);
	Time operator-(const Time& l, const Time& r);
	Time operator*(const Time& l, const Time& r);
	Time operator/(const Time& l, const Time& r);
	Time operator%(const Time& l, const Time& r);

	/// <summary>
	/// Wrapper around a std::chrono::high_resolution_clock to measure time elapsing
	/// </summary>
	class Clock
	{

	public:
		Clock();
		Time GetElapsed() const;
		Time Restart();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	};

	typedef std::chrono::seconds seconds;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::microseconds us;

}