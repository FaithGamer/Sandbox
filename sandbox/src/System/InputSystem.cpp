#include "pch.h"

#include "Sandbox/Input/Inputs.h"
#include "Sandbox/System/InputSystem.h"

namespace Sandbox
{
	InputSystem::InputSystem()
	{
		SetPriority(-10000);
	}
	bool InputSystem::OnEvent(SDL_Event& event)
	{
		return Inputs::GetCurrentInputMap()->OnEvent(event);
	}

	int InputSystem::GetUsedMethod()
	{
		return System::Method::Event;
	}
}