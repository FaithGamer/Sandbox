#pragma once
#include <math.h>
#ifndef PI
#define PI 3.141592653589793
#endif
namespace Sandbox
{
	namespace Easing
	{
		float SineInOut(float t);
		float SineIn(float t);
		float SineOut(float t);
		float QuadInOut(float t);
		float QuadIn(float t);
		float QuadOut(float t);
		float CubicInOut(float t);
		float CubicIn(float t);
		float CubicOut(float t);
	}
}