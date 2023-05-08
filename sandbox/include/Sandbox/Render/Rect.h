#pragma once

#include "Sandbox/Vec.h"

namespace Sandbox
{
	class Rect
	{
	public:
		Rect() : left(0), top(0), width(0), height(0)
		{}
		Rect(float Left, float Top, float Width, float Height)
			: left(Left), top(Top), width(Width), height(Height)
		{}

		float left;
		float top;
		float width;
		float height;
	};
}
