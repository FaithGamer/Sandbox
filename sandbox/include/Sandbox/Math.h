#pragma once

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
	}
}

#include "Math.tpp"