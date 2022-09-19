#pragma once

#include "Render/Rect.h"
using namespace sandbox;

class RenderViewSDL
{
public:
	RenderViewSDL();
	RenderViewSDL(Recti rect);
	Recti m_rect;
};

