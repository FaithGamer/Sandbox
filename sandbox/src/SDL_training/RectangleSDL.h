#pragma once

#include "SDL.h"

#include "RenderWindowSDL.h"
#include "Color.h"
#include "Rect.h"

class RectangleSDL
{
public:

	RectangleSDL();
	RectangleSDL(Recti rect);

	void SetColor(Color col);
	void SetSize(Vec2i size);
	void SetPosition(Vec2i pos);
	void Draw(RenderWindowSDL& window);

	Color m_color;
	SDL_Rect m_sdlrect;
};

