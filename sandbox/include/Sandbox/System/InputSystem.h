#pragma once

#include "Sandbox/System/System.h"

namespace Sandbox
{
	class InputSystem : public System
	{
		InputSystem();
		bool OnEvent(SDL_Event& event);
		int GetUsedMethod();
	};
}
