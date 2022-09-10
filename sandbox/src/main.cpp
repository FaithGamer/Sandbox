#include <iostream>
#include <cassert>
#include <any>

int main()
{

	float monFloat = 14;

	std::any Anything = monFloat;
	
	if (Anything.type() == std::make_any<float>().type())
	{
		std::cout << "oui" << std::endl;
	}
}