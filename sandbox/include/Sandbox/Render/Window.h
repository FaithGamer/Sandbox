#pragma once

#include <string>
#include <glad/glad.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "Sandbox/Internal/Singleton.h"
#include "Sandbox/Core/Vec.h"
#include "RenderTarget.h"
#include "Sandbox/Core/Signal.h"

namespace Sandbox
{
	/// @brief The SDL window and OpenGL context
	class Window : public Singleton<Window>, public RenderTarget
	{
	public:
		static void SetWindowSize(Vec2u size);
		static void SetFullScreen(bool fullscreen);
		static void SetVsync(bool vsync);
		static void ClearWindow();
		static void RenderWindow();
		static void SetSize(float width, float height);
		static void ShowCursor(bool showCursor);
		static bool IsInitialized();
		static Vec2u GetSize();
		static float GetAspectRatio();
		static SDL_GLContext GetSDL_GLContext();
		static SDL_Window* GetSDLWindow();
		static SignalSender<Vec2u>* GetResizeSignal();

		void SetSize(Vec2u size) override;
		void Bind() override;
		void Clear() override;
		
		SignalSender<Vec2u> ResizeSignal;
	private:
		friend Engine;
		friend Singleton<Window>;
		Window();
		void Init(std::string name, Vec2u size);
		bool m_initialized;
		SDL_Window* m_window;
		SDL_GLContext m_glContext;

		Vec2u m_size;
	};
}
