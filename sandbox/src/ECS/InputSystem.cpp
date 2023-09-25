#include "pch.h"

#include "Sandbox/Input/Inputs.h"
#include "Sandbox/ECS/InputSystem.h"

namespace Sandbox
{
	InputSystem::InputSystem()
	{
		SetPriority(-10000);
	}

	bool InputSystem::OnEvent(SDL_Event& event, bool handledByImGui)
	{
		bool eventConsumed = false;
		for(auto& inputMap : Inputs::GetInputMaps())
		{
			if (!inputMap->IsActive())
				continue;

			eventConsumed = inputMap->OnEvent(event, handledByImGui);
		}
		return eventConsumed;
	}

	int InputSystem::GetUsedMethod()
	{
		return System::Method::Event;
	}
}