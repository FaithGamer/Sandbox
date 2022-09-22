#include "pch.h"
#include "WindowGLContext.h"

#include "Log.h"

namespace sandbox
{
	bool WindowGLContext::windowExist = false;

	WindowGLContext::WindowGLContext(std::string name, Vec2i size)
	{
		//Make sure there is only one window existing at all time (could make this a Singleton)
		ASSERT_LOG_ERROR(!windowExist, "WindowGLContext already exist");

		//Initializing SDL
		ASSERT_LOG_ERROR((SDL_Init(SDL_INIT_VIDEO) == 0),
			LogSDLError("WindowGLContext::WindowGLContext, Couldn't initialize SDL"));

		//Load default OpenGL library
		SDL_GL_LoadLibrary(NULL);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Creating SDL Window
		m_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, SDL_WINDOW_OPENGL);
		ASSERT_LOG_ERROR(m_window, LogSDLError("WindowGLContext::WindowGLContext, Cannot create SDL window"));


		//Creating OpenGL Context with SDL
		m_glContext = SDL_GL_CreateContext(m_window);
		ASSERT_LOG_ERROR(m_glContext, LogSDLError("WindowGLContext::WindowGLContext, Cannot create OpenGL Context"));

		//Loading OpenGL Functions addresses
		ASSERT_LOG_ERROR((bool)(gladLoadGLLoader(SDL_GL_GetProcAddress)), "WindowGLContext::WindowGLContext, Couldn't initialize GLAD");

		//Logging additional information
		auto c = glGetString(GL_VENDOR);
		LOG_INFO("OpenGL Loaded.");
		LOG_INFO("Version: " + std::string((const char*)glGetString(GL_VERSION)));
		LOG_INFO("Renderer: " + std::string((const char*)glGetString(GL_RENDERER)));
		LOG_INFO("Vendor: " + std::string((const char*)glGetString(GL_VENDOR)));

		int maxVertAttrib = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertAttrib);

		LOG_INFO("Max. Vertex attributes: " + std::to_string(maxVertAttrib));

		glViewport(0, 0, size.x, size.y);
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);

		SDL_GL_SetSwapInterval(0);

		windowExist = true;
	}

	void WindowGLContext::SetSize(Vec2i size)
	{
		SDL_SetWindowSize(m_window, size.x, size.y);
		glViewport(0, 0, size.x, size.y);
	}

	void WindowGLContext::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void WindowGLContext::Render()
	{
		SDL_GL_SwapWindow(m_window);
	}
}
