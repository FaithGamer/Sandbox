#pragma once

#include "SDL.h"
//TO DO: delete this
/// simple RectangleSDL class with positionand size
namespace Sandbox
{
	template <class T>
	class Rect
	{

	public:
		Rect() : x(0), y(0), w(0), h(0) {}
		Rect(T posx, T posy, T width, T height) : x(posx), y(posy), w(width), h(height) {}
		Rect(SDL_Rect sdlrect) : x(sdlrect.x), y(sdlrect.y), w(sdlrect.w), h(sdlrect.h) {}

		T x;
		T y;
		T w;
		T h;

		template <class U>
		Rect<T>& operator=(const Rect<U>& rect);
		SDL_Rect ConvertToSdlRect();
		SDL_FRect ConvertToSdlFRect();

	};



	typedef Rect<int> Recti;
	typedef Rect<float> Rectf;

}

#include "Rect.tpp"