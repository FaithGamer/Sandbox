#include "pch.h"
#include "Sandbox/Core/Easing.h"

namespace Sandbox
{
	float Easing::SineInOut(float t)
	{
		return 0.5 * (1 + sin(3.1415926 * (t - 0.5)));

	}
	float Easing::SineIn(float t)
	{
		return sin(1.5707963 * t);
	}
	float Easing::SineOut(float t)
	{
		return 1 + sin(1.5707963 * (--t));
	}
	float Easing::QuadInOut(float t)
	{
		return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
	}
	float Easing::QuadIn(float t)
	{
		return t * t;
	}
	float Easing::QuadOut(float t)
	{
		return t * (2 - t);
	}
	float Easing::CubicInOut(float t)
	{
		return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
	}
	float Easing::CubicIn(float t)
	{
		return t * t * t;
	}
	float Easing::CubicOut(float t)
	{
		return 1 + (--t) * t * t;
	}
}