#pragma once
#include "Sandbox/ECS/System.h"
//must associate every new body with their entity through user data

namespace Sandbox
{
	class PhysicsSystem : public System
	{
	public:
		void OnStart() override;
		int GetUsedMethod() override;
		void OnAddBody(ComponentSignal signal);

	};
}