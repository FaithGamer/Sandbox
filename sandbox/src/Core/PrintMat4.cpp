#pragma once
#include "pch.h"


#include "Sandbox/Core/Print.h"

namespace Sandbox
{
	namespace Print
	{
		void mat4(Mat4 mat)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					std::cout << mat[j][i] << "       ";
				}
				std::cout << std::endl;
			}
		}
	}
}