#pragma once

#include "Input.h"

namespace Sandbox
{
	class WheelInput
	{
	public:
		InputType GetType() const { return InputType::Scroll; }


		bool up;

	};
}