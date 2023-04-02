#pragma once

#include "Sandbox/Input/Input.h"

namespace Sandbox
{
	class WheelInput
	{
	public:
		InputType GetType() const { return InputType::Scroll; }


		bool up;

	};
}