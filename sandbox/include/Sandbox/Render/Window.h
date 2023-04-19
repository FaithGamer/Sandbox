#pragma once

#include <string>
#include <glad/glad.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "Sandbox/Singleton.h"
#include "Sandbox/Vec.h"
#include "RenderTarget.h"
#include "Sandbox/Signal.h"

namespace Sandbox
{
	/// @brief The SDL window and OpenGL context
	class Window : public Singleton<Window>, public RenderTarget
	{
	public:
		static void SetWindowSize(Vec2u size);
		static void ClearWindow();
		static void RenderWindow();
		static Vec2u GetSize();
		static SDL_GLContext GetSDL_GLContext();
		static SDL_Window* GetSDLWindow();

		void Clear() override;
		void SetSize(Vec2u size) override;
		void Bind() override;

		SignalSender<Vec2u> resizeSignal;
	private:
		friend Engine;
		
		void Load(std::string name, Vec2u size);
		SDL_Window* m_window;
		SDL_GLContext m_glContext;

		Vec2u m_size;
	};
}
