#include "pch.h"
#include "ToolboxTest.h"
#include "Tools/Toolbox.h"

using namespace Sandbox;

namespace Sandbox
{
	void TestToolbox()
	{
		LOG_INFO("Test Toolbox");

		std::vector<int> VV{ 1, 1,1,1,1,16, 5,1,1,1,1, 4, 1,1,1,1,1, 1, 7, 4, 1,1 , 1,1,1,1,1 };
		int K = Toolbox::VectorRemove(1, VV);
		std::cout << K << std::endl;
		for (auto j : VV)
		{
			std::cout << j << std::endl;
		}
	}
}