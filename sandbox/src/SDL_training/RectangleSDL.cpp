#include "pch.h"
#include "RectangleSDL.h"

RectangleSDL::RectangleSDL() : m_sdlrect{ 0, 0, 0, 0 }
{

}

RectangleSDL::RectangleSDL(Recti rect)
{
	m_sdlrect = rect.ConvertToSdlRect();
}

void RectangleSDL::SetColor(Color col)
{
	m_color = col;
}

void RectangleSDL::SetSize(Vec2i size)
{
	m_sdlrect.w = size.x;
	m_sdlrect.h = size.y;
}

void RectangleSDL::SetPosition(Vec2i pos)
{
	m_sdlrect.x = pos.x;
	m_sdlrect.y = pos.y;
}

void RectangleSDL::Draw(RenderWindowSDL& window)
{
	SDL_Rect destRect(m_sdlrect.x - window.m_view.m_rect.x, m_sdlrect.y - window.m_view.m_rect.y, m_sdlrect.w, m_sdlrect.h);
	SDL_SetRenderDrawColor(window.renderer, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(window.renderer, &destRect);
}