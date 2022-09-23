#pragma once
#include "pch.h"


#include "Tools/print.h"

namespace sandbox
{
	namespace print
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