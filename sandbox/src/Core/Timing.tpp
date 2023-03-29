#include "Time.h"

namespace Sandbox
{
	////////////////////////////
	/// Time ///////////////////
	////////////////////////////
	constexpr Time::Time() : m_microseconds(std::chrono::microseconds::zero())
	{

	}

	template <typename rep, typename ratio>
	constexpr Time::Time(const std::chrono::duration<rep, ratio>& duration)
		: m_microseconds(std::chrono::duration_cast<std::chrono::microseconds>(duration))
	{

	}

	constexpr Time::Time(float seconds) : m_microseconds((int64_t)(seconds * 1000000))
	{

	}

	template <typename chrono_duration>
	constexpr chrono_duration Time::GetDuration() const
	{
		return std::chrono::duration_cast<chrono_duration>(m_microseconds);
	}

	constexpr Time::operator float() const
	{
		return (float)m_microseconds.count()/1000000;
	}




	constexpr Time& operator-=(Time& l, const Time& r)
	{
		return l = Time(l.GetDuration() - r.GetDuration());
	}
	constexpr Time& operator*=(Time& l, const Time& r)
	{
		return l = Time(l.GetDuration() * r.GetDuration());
	}
	constexpr Time& operator/=(Time& l, const Time& r)
	{
		return l = Time((float)l.GetDuration().count() / (float)r.GetDuration().count());
	}
	constexpr Time& operator%=(Time& l, const Time& r)
	{
		return l = Time(l.GetDuration() % r.GetDuration());
	}


	constexpr Time operator-(const Time& l, const Time& r)
	{
		return Time(l.GetDuration() - r.GetDuration());
	}
	constexpr Time operator*(const Time& l, const Time& r)
	{
		return Time(l.GetDuration() * r.GetDuration());
	}
	constexpr Time operator/(const Time& l, const Time& r)
	{
		return Time((float)l.GetDuration().count() / (float)r.GetDuration().count());
	}
	constexpr Time operator%(const Time& l, const Time& r)
	{
		return Time(l.GetDuration() % r.GetDuration());
	}
}