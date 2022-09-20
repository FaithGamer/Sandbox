#include "pch.h"
#include "Vertex.h"

namespace sandbox
{
	Vertex::Vertex(Vec2f Pos)
	{
		data[0] = (float)Pos.x;
		data[1] = (float)Pos.y;
		data[2] = 0.f;
		data[3] = 1.f;
		data[4] = 1.f;
		data[5] = 1.f;
		data[6] = 1.f;
	}

	Vertex::Vertex(Vec2f Pos, Color Color)
	{
		data[0] = (float)Pos.x;
		data[1] = (float)Pos.y;
		data[2] = 0.f;
		data[3] = (float)Color.r / 255;
		data[4] = (float)Color.g / 255;
		data[5] = (float)Color.b / 255;
		data[6] = (float)Color.a / 255;
	};

}

float* convertToFloatArray()
{
	return new float[] {1, 2, 3};
}
