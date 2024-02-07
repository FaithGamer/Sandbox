#pragma once
#include <math.h>
#ifndef PI
#define PI 3.141592653589793
#endif
namespace Sandbox
{
	namespace Easing
	{
		inline float SineInOut(float t)
		{
			return -0.5 * (cos(PI * t) - 1.0);
		}
	}
}