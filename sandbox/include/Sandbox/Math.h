#pragma once

#include "Sandbox/Vec.h"

namespace Sandbox
{
	namespace Math
	{
		/// <summary>
		/// Scale a value ranged in between two number, to another range in between two other numbers.
		/// </summary>
		/// <returns>The value scaled to the new range</returns>
		template <typename T>
		T ScaleRangeTo(T value, T newMin, T newMax, T oldMin, T oldMax);

		inline Vec2f AngleToVec(float degrees)
		{
			return { cos(degrees), sin(degrees) };
		}

		inline float VecToAngle(Vec2f unitVector)
		{
			return glm::degrees(atan2(unitVector.y, unitVector.x));
		}
	}
}

#include "Math.tpp"