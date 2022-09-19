#pragma once
#include "SDL.h"

namespace sandbox
{
	class Color
	{
	public:
		Color() : r(255), g(255), b(255), a(255) {}
		Color(Uint8 red, Uint8 green, Uint8 blue) : r(red), g(green), b(blue), a(255) {}
		Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) : r(red), g(green), b(blue), a(alpha) {}

		Uint8 r;
		Uint8 g;
		Uint8 b;
		Uint8 a;
	};
}