#pragma once

#include "Vec.h"
#include "Color.h"

namespace Sandbox
{
	class Vertex
	{
	public:
		Vertex(Vec2f Pos);
		Vertex(Vec2f Pos, Color Color);
		Vertex(Vec2f Pos, Color Color, Vec2f texCoords);

	private:
		std::vector<float> m_data;
	};
}