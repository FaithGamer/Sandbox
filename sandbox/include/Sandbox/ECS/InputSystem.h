#pragma once

#include "Sandbox/ECS/System.h"

namespace Sandbox
{
	class InputSystem : public System
	{
	public:
		InputSystem();
		bool OnEvent(SDL_Event& event);
		int GetUsedMethod();
	};
}
