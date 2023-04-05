#pragma once

#include <string>
#include <glad/glad.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "Sandbox/Singleton.h"
#include "Sandbox/Vec.h"

namespace Sandbox
{
	/// @brief The SDL window and OpenGL context
	class Window : public Singleton<Window>
	{
	public:
		static void SetSize(Vec2u size);
		static void Clear();
		static void Render();
		static Vec2u GetSize();
		static SDL_GLContext GetSDL_GLContext();
		static SDL_Window* GetSDLWindow();
	private:
		friend Engine;
		
		void Load(std::string name, Vec2u size);
		SDL_Window* m_window;
		SDL_GLContext m_glContext;

		Vec2u m_size;
	};
}
