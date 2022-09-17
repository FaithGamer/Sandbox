#include "pch.h"

#include "WindowGLContext.h"

void Init()
{

}

int main(int argc, char* argv[])
{

	WindowGLContext c("hello window", Vec2i(10, 10));
	WindowGLContext b("hello window", Vec2i(10, 10));

	std::cout << "hello" << std::endl;
	
	return 0;
}