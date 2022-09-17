#pragma once
#include "Rect.h"

template <class T> template <class U>
Rect<T>& Rect<T>::operator=(const Rect<U>& rect)
{
	x = (T)rect.x;
	y = (T)rect.y;
	w = (T)rect.w;
	h = (T)rect.h;
	return *this;
}

template <class T>
SDL_Rect Rect<T>::ConvertToSdlRect()
{
	return SDL_Rect{ (int)x, (int)y, (int)w, (int)h };
}

template <class T>
SDL_FRect Rect<T>::ConvertToSdlFRect()
{
	return SDL_Rect{ (float)x, (float)y, (float)w, (float)h };
}