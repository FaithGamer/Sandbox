#include "pch.h"

#include "glad/glad.h"
#include "SDL_opengl.h"

#include "Render/Vec2.h"
#include "TextureSDL.h"
#include "Render/Color.h"
#include "RenderWindowSDL.h"
#include "RectangleSDL.h"
#include "SpriteSDL.h"
#include "RenderViewSDL.h"

#include "std_macros.h"

using namespace sandbox;

void InitPng()
{
	if (!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)
	{
		LogIMGError("TestPng IMG_Init");
	}
}


void TestRenderer()
{
	InitPng();
	RenderWindowSDL window("App", Vec2i(640, 480));
	TextureSDL tex("image.png", window.renderer);


	SDL_Event e;
	bool play = true;

	while (play)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				play = false;
			}
		}

		window.Clear(Color(0, 0, 0, 0));
		SDL_RenderCopy(window.renderer, tex.texture, NULL, NULL);
		window.Render();
	}

}

void TestViewport()
{
	RenderWindowSDL window("App", Vec2i(640, 480));
	SDL_Event e;

	sptr<TextureSDL> tex = makesptr<TextureSDL>("image.png", window.renderer);

	SpriteSDL sprite(tex);
	RenderViewSDL view(Recti(100, 0,100, 400));
	window.SetRenderViewSDL(view);

	Recti rect1(45, 98, 45, 10);

	Recti rect2;
	rect2 = rect1;

	bool play = true;
	while (play)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				play = false;
			}
		}

		window.Clear(Color(0, 0, 0, 0));
		sprite.Draw(window);
	
		window.Render();
	}
}

void TestSpriteSDL()
{
	RenderWindowSDL window("App", Vec2i(640, 480));
	SDL_Event e;

	std::shared_ptr<TextureSDL> tex = std::make_shared<TextureSDL>("image.png", window.renderer);
	SpriteSDL sprite(tex);
	sprite.m_position.x = 50;
	//window.SetViewPort(SDL_Rect(50, 50, 100, 500));

	bool play = true;
	while (play)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				play = false;
			}
		}

		window.Clear(Color(0, 0, 0, 0));
		sprite.Draw(window);

		window.Render();
	}
}


int Play()
{

	int init_error = SDL_Init(SDL_INIT_VIDEO);
	if (init_error < 0)
	{
		std::cerr << "SDL_init error " << SDL_GetError() << std::endl;
		return 1;
	}

	TestViewport();

	IMG_Quit();
	SDL_Quit();
	return 0;
}

