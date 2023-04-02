#include "Sandbox/Math.h"

namespace Sandbox
{
	namespace Math
	{
		template <typename T>
		T ScaleRangeTo(T value, T newMin, T newMax, T oldMin, T oldMax)
		{
			T oldRange = (oldMax - oldMin);
			T newRange = (newMax - newMin);
			return (((value - oldMin) * newRange) / oldRange) + newMin;
		}
	}
}