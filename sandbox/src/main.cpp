#include <iostream>
#include <cassert>
#include <any>
#include <stdio.h>

#include "SDL.h"

int main(int argc, char** argv)
{

	float monFloat = 14;

	std::any Anything = monFloat;
	
	if (Anything.type() == std::make_any<float>().type())
	{
		std::cout << "oui" << std::endl;
	}

	return 0;
}