#include "pch.h"
#include <cassert>
#include "RenderWindowSDL.h"
#include "Log.h"

bool RenderWindowSDL::windowCreated = false;

RenderWindowSDL::RenderWindowSDL(std::string name, Vec2i resolution)
{
	assert(!windowCreated);

	window = CreateRenderWindowSDL(name, resolution);
	auto surface = SDL_GetWindowSurface(window);
	renderer = CreateRenderer(window);
	windowCreated = true;
	RenderViewSDL defaultView(Recti(0, 0, resolution.x, resolution.y));
	SetRenderViewSDL(defaultView);
}

void RenderWindowSDL::Clear(Color col)
{
	SDL_SetRenderDrawColor(renderer, col.r, col.r, col.g, col.a);
	SDL_RenderClear(renderer);
}

void RenderWindowSDL::Render()
{
	//SDL_Rect sdlrect{ m_view.m_rect.x, m_view.m_rect.y, m_view.m_rect.w, m_view.m_rect.h };
	SDL_RenderSetViewport(renderer, &m_viewport);
	SDL_RenderPresent(renderer);
}

void RenderWindowSDL::SetRenderViewSDL(RenderViewSDL view)
{
	m_view = view;
	m_viewport = SDL_Rect{ view.m_rect.x, view.m_rect.y, view.m_rect.w, view.m_rect.h };
	SDL_RenderSetViewport(renderer, &m_viewport);
}

RenderWindowSDL::~RenderWindowSDL()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	windowCreated = false;
}

SDL_Window* RenderWindowSDL::CreateRenderWindowSDL(std::string name, Vec2i resolution)
{
	SDL_Window* window = SDL_CreateWindow(std::string("Application").c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution.x, resolution.y, SDL_WINDOW_SHOWN);

	if (!window)
	{
		LogSDLError("RenderWindowSDL::CreateRenderWindowSDL");
	}

	return window;
}

SDL_Renderer* RenderWindowSDL::CreateRenderer(SDL_Window* window)
{
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		LogSDLError("RenderWindowSDL::CreateRenderer");
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}

	return renderer;
}