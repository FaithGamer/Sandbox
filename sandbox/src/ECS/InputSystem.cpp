#include "pch.h"

#include "Sandbox/Input/Inputs.h"
#include "Sandbox/ECS/InputSystem.h"

namespace Sandbox
{
	InputSystem::InputSystem()
	{
		SetPriority(-10000);
	}

	bool InputSystem::OnEvent(SDL_Event& event)
	{
		if (Inputs::HasInputMap())
		{
			InputMap& inputs = Inputs::GetCurrentInputMap();
			return inputs.OnEvent(event);
		}
		return false;
	}

	int InputSystem::GetUsedMethod()
	{
		return System::Method::Event;
	}
}