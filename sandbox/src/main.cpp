#include <iostream>
#include <cassert>
#include <any>
#include <stdio.h>

#include <SDL.h>
#

int main(int argc, char** argv)
{

	SDL_Window* window = nullptr;
	SDL_Surface* surface = nullptr;

	int init_error = SDL_Init(SDL_INIT_VIDEO);
	if (init_error < 0)
	{
		std::cerr << "SDL_init error " << SDL_GetError() << std::endl;
		return 1;
	}

	window = SDL_CreateWindow(std::string("Application").c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 620, 480, SDL_WINDOW_SHOWN);

	if (!window)
	{
		std::cerr << "SDL_Window error " << SDL_GetError() << std::endl;
	}

	surface = SDL_GetWindowSurface(window);

	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 60, 6, 6));

	SDL_UpdateWindowSurface(window);

	SDL_Delay(2000);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}