#pragma once

#include "SDL.h"

#include "RenderWindowSDL.h"
#include "Render/Color.h"
#include "Render/Rect.h"
#include "Render/Vec2.h"

using namespace sandbox;

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

