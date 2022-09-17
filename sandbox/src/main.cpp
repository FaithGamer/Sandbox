#include "pch.h"
#include "WindowGLContext.h"
#include "Log.h"



int main(int argc, char* argv[])
{
	Log::Init();
	LOG_INFO("Logger initialiazed");

	WindowGLContext window("hello window", Vec2i(500, 500));


	SDL_Event e;
	bool run = true;
	while (run)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				run = false;
			}
		}

		window.Clear();
		window.Render();
	}

	
	return 0;
}