#pragma once

#include "SDL.h"
#include "SDL_image.h"


#include "Render/Vec2.h"
#include "Log.h"

using namespace sandbox;

class TextureSDL
{
public:

	TextureSDL()
	{
		texture = nullptr;
	}

	TextureSDL(std::string path, SDL_Renderer* renderer) : TextureSDL()
	{
		texture = LoadFromFile(path, renderer);
	}

	TextureSDL& operator=(const TextureSDL& other) = delete;

	~TextureSDL()
	{
		Destroy();
	}
	
	SDL_Texture* LoadFromFile(std::string path, SDL_Renderer* renderer)
	{
		Destroy();

		SDL_Surface* surface = IMG_Load(path.c_str());
		if (surface == nullptr)
		{
			LogIMGError("TextureSDL::LoadFromFile loading image, " + path);
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == nullptr)
		{
			LogSDLError("TextureSDL::LoadFromFile, creating texture from surface, " + path);
		}
		else
		{
			size.x = surface->w;
			size.y = surface->h;
		}

		SDL_FreeSurface(surface);

		return texture;
	}
	bool IsLoaded()
	{
		if (texture == nullptr)
		{
			return false;
		}
		return true;
	}
	void Destroy()
	{
		if (texture != nullptr)
		{
			size.x = 0;
			size.y = 0;
			SDL_DestroyTexture(texture);
		}
	}

	Vec2i size;
	SDL_Texture* texture;
private:

	

	
}; 
