#include "pch.h"
#include "WindowGLContext.h"


bool WindowGLContext::windowExist = false;

WindowGLContext::WindowGLContext(std::string name, Vec2i resolution)
{
	assert(!windowExist);
	windowExist = true;
}


