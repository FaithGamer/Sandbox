#pragma once

#include <glad/glad.h>
#include <SDL.h>

#include "Vec2.h"

class WindowGLContext
{
public:
	WindowGLContext(std::string name, Vec2i resolution);
	static bool windowExist;
	SDL_Window* m_SDLwindow;
	SDL_GLContext m_glcontext;
};

