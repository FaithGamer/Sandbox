#pragma once
#include "Sandbox/Core/Vec.h"
#include <SDL/SDL.h>


namespace Sandbox
{
	void BeginImGui();
	void EndImGui(Vec2u windowSize);
	void LoadImGui(SDL_Window* sdlWindow, SDL_GLContext sdlGlContext, bool lightTheme = false);
	void ExitImGui();
}