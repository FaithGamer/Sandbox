#pragma once

#include "Rect.h"

class RenderViewSDL
{
public:
	RenderViewSDL();
	RenderViewSDL(Recti rect);
	Recti m_rect;
};

