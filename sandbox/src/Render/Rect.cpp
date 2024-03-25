#include "pch.h"
#include "Sandbox/Render/Rect.h"

namespace Sandbox
{
	bool Rect::PointInside(Vec2f point) const
	{
		if (point.x < left
			|| point.x > left + width
			|| point.y > top
			|| point.y < top - height)
			return false;
		return true;
	}

	bool Rect::PointInside(float x, float y) const
	{
		return PointInside({ x, y });
	}
}