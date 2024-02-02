#pragma once
#include <math.h>
#include "Sandbox/Vec.h"

namespace Sandbox
{
	namespace Math
	{
		///@brief Scale a value ranged in between two number, to another range in between two other numbers.
		template <typename T>
		T ScaleRangeTo(T value, T oldMin, T oldMax, T newMin, T newMax)
		{
			T oldRange = (oldMax - oldMin);
			T newRange = (newMax - newMin);
			return (((value - oldMin) * newRange) / oldRange) + newMin;
		}

		/// @brief Return direction of degrees in vector
		inline Vec2f AngleToVec(float degrees)
		{
			return { cos(degrees), sin(degrees) };
		}

		/// @brief Return direction of vector in degrees
		inline float VecToAngle(Vec2f vector)
		{
			return glm::degrees(atan2(vector.y, vector.x));
		}

		template <typename T>
		inline constexpr T Min(T lhs, T rhs)
		{
			return std::min(lhs, rhs);
		}

		template <typename T>
		inline constexpr T Max(T lhs, T rhs)
		{
			return std::max(lhs, rhs);
		}

		inline float Lerp(float min, float max, float t)
		{
			return min * (1 - t) + max * t;
		}

		inline float Clamp(float value, float min, float max)
		{
			const float t = value < min ? min : value;
			return t > max ? max : t;
		}
		inline float Clamp01(float value)
		{
			const float t = value < 0 ? 0 : value;
			return t > 1 ? 1 : t;
		}
		inline float Sin(float degrees)
		{
			return sin(glm::radians(degrees));
		}
		inline float Cos(float degrees)
		{
			return cos(glm::radians(degrees));
		}
		///@brief Loops the value t, so that it is never larger than length and never smaller than 0.
		inline float Repeat(float t, float length)
		{
			return Clamp(t - std::floor(t / length) * length, 0.0f, length);
		}
		template <typename T>
		inline int Sign(T val)
		{
			return (T(0) < val) - (val < T(0));
		}
		///@brief Calculates the shortest difference between two given angles.
		inline float DeltaAngle(float current, float target)
		{
			float delta = Repeat((target - current), 360.0F);
			if (delta > 180.0F)
				delta -= 360.0F;
			return delta;
		}
		inline float MoveTowards(float current, float target, float maxDelta)
		{
			if (abs(target - current) <= maxDelta)
				return target;
			return current + Sign(target - current) * maxDelta;
		}
		inline float MoveTowardsAngle(float current, float target, float maxDelta)
		{
			float deltaAngle = DeltaAngle(current, target);
			if (-maxDelta < deltaAngle && deltaAngle < maxDelta)
				return target;
			target = current + deltaAngle;
			return MoveTowards(current, target, maxDelta);
		}
	}
}

#include "Math.tpp"