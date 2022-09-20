#pragma once

namespace sandbox
{
	class Color
	{
	public:
		Color() : r(255), g(255), b(255), a(255) {}
		Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue), a(255) {}
		Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) {}

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};
}