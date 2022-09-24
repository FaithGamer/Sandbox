#pragma once
#include <chrono>

namespace sandbox
{
    /// <summary>
    /// Wrapper around a std::chrono::microseconds to represent time duration
    /// </summary>
    class Time
    {
    public:
        constexpr Time();

        constexpr Time(const float seconds);

        template <typename rep, typename ratio>
        constexpr Time(const std::chrono::duration<rep, ratio>& duration);

        template <typename chrono_duration = std::chrono::microseconds>
        constexpr chrono_duration GetDuration() const;

    private:

        std::chrono::microseconds m_microseconds;
    };

    constexpr Time& operator+=(Time& l, const Time& r);
    constexpr Time& operator-=(Time& l, const Time& r);
    constexpr Time& operator*=(Time& l, const Time& r);
    constexpr Time& operator/=(Time& l, const Time& r);
    constexpr Time& operator%=(Time& l, const Time& r);

    constexpr Time operator+(const Time& l, const Time& r);
    constexpr Time operator-(const Time& l, const Time& r);
    constexpr Time operator*(const Time& l, const Time& r);
    constexpr Time operator/(const Time& l, const Time& r);
    constexpr Time operator%(const Time& l, const Time& r);

    /// <summary>
    /// Wrapper around a std::chrono::highresolution clock to measure time elapsing
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

#include "Timing.tpp"