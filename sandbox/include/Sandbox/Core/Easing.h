#pragma once
#include <math.h>
#ifndef PI
#define PI 3.141592653589793
#endif
namespace Sandbox
{
	namespace Easing
	{
		inline float SineInOut(float t);
		inline float SineIn(float t);
		inline float SineOut(float t);
		inline float QuadInOut(float t);
		inline float QuadIn(float t);
		inline float QuadOut(float t);
		inline float CubicInOut(float t);
		inline float CubicIn(float t);
		inline float CubicOut(float t);
	}
}