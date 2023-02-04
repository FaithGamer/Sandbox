#pragma once

#include "iInput.h"

namespace Sandbox
{
	class WheelInput
	{
	public:
		InputType GetType() const { return InputType::wheel; }

		/// State
		bool up;

		//Action is always considered to be the mouse wheel atm
	};
}