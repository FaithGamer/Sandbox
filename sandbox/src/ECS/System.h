#pragma once
#include "std_macros.h"
#include "Core/Time.h"
#include <SDL.h>

namespace Sandbox
{
	class System
	{
	public:
		virtual ~System() {};
		virtual void Update(Time time) {};
		virtual void FixedUpdate() {};
	};

	class EventSystem : public System
	{
	public:
		virtual ~EventSystem() {};
		virtual void OnEvent(SDL_Event& event) = 0;
	};
}