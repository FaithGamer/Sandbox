#pragma once

#include <glad/glad.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "Singleton.h"
#include "Vec.h"

struct EngineParameters;

namespace Sandbox
{
	/// @brief The SDL window and OpenGL context
	class Window : public Singleton<Window>
	{
	public:
		static void SetSize(Vec2i size);
		static void Clear();
		static void Render();
	private:
		friend void Launch(const EngineParameters& parameters);
		
		void Load(std::string name, Vec2u size);
		SDL_Window* m_window;
		SDL_GLContext m_glContext;
	};
}
