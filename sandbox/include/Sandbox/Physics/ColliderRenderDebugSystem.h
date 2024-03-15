#pragma once

#include "Sandbox/ECS/System.h"

namespace Sandbox
{
	class ColliderRenderDebugSystem : public System
	{
	public:
		ColliderRenderDebugSystem();
		void OnStart() override;
		void OnUpdate(Time delta) override;
		void OnRender() override;
		int GetUsedMethod() override;
		void OnRemove() override;

		void OnAddKinematicBody(ComponentSignal signal);
		void OnAddStaticBody(ComponentSignal signal);
	private:
		uint32_t m_debugLayer;
		std::unordered_set<EntityId> m_newStaticBodies;
		std::unordered_set<EntityId> m_newKinematicBodies;
	};
}

