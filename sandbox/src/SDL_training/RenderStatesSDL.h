#pragma once

#include "Render/Vec2.h"
#include "RenderViewSDL.h"

using namespace sandbox;

class RenderStatesSDL
{
public:
	RenderStatesSDL() : position(0, 0), rotation(0), mirrored(0)
	{

	}
	RenderStatesSDL(Vec2i pos, float rotationDegree, int mirror) : position(pos), rotation(rotationDegree), mirrored(mirror)
	{

	}
	Vec2i position;
	float rotation;
	int mirrored;

};
