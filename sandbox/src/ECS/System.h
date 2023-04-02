#pragma once
#include "std_macros.h"
#include "Core/Time.h"
#include <SDL/SDL.h>

namespace Sandbox
{
	

	class System
	{
	public:

		typedef enum
		{
			Update = 1,
			FixedUpdate = 2,
			Event = 3,
			ImGui = 4
		}Method;

		virtual ~System() {}
		virtual void OnStart() {}
		virtual void OnUpdate(Time time) {}
		virtual void OnFixedUpdate() {}
		virtual void OnImGui() {}
		virtual bool OnEvent(SDL_Event& event) {}
		virtual void OnDestroy() {}

		virtual int GetUsedMethod() { return Update | FixedUpdate | Event | ImGui; }
		
		virtual int GetPriority() { return 0; }

	};
}