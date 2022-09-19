#pragma once

#include "SDL.h"
#include "Render/Color.h"
#include "Render/Vec2.h"
#include "RenderViewSDL.h"

using namespace sandbox;

class RenderWindowSDL
{
public:

	RenderWindowSDL(std::string name, Vec2i resolution);

	void Clear(Color col);

	void Render();

	void SetRenderViewSDL(RenderViewSDL view);
	~RenderWindowSDL();
	SDL_Window* window;
	SDL_Renderer* renderer;
	RenderViewSDL m_view;
private:
	SDL_Window* CreateRenderWindowSDL(std::string name, Vec2i resolution);
	SDL_Renderer* CreateRenderer(SDL_Window* window);
	SDL_Rect m_viewport;


	static bool windowCreated;

};


