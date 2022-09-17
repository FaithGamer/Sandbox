#include "pch.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Log.h"

void LogSDLError(std::string str)
{
	std::cerr << "SDL error: " << str << " " << SDL_GetError() << std::endl;
}
void LogIMGError(std::string str)
{
	std::cerr << "IMG_Error " << str << " " << IMG_GetError() << std::endl;
}
