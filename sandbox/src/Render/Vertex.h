#pragma once

#include "Vec2.h"
#include "Color.h"

namespace sandbox
{
	struct Vertex
	{
		Vertex(Vec2f Pos);
		Vertex(Vec2f Pos, Color Color);

		float data[7];
	};


}