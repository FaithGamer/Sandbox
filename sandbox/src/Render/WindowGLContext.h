#pragma once

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Vec.h"

namespace sandbox
{
	/// <summary>
	/// Initialize SDL to work with OpenGL, create a unique window.
	/// </summary>
	class WindowGLContext
	{
	public:
		WindowGLContext(std::string name, Vec2i size);
		void SetSize(Vec2i size);
		void Clear();
		void Render();
		static bool windowExist;
	private:
		SDL_Window* m_window;
		SDL_GLContext m_glContext;
	};
}
