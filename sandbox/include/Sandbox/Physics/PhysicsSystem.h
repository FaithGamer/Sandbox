#pragma once
#include "Sandbox/ECS/System.h"
//must associate every new body with their entity through user data

namespace Sandbox
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();
		void OnStart() override;
		void OnUpdate(Time deltaTime) override;
		int GetUsedMethod() override;
		void OnAddKinematicBody(ComponentSignal signal);
		void OnAddStaticBody(ComponentSignal signal);

	};
}