#pragma once

#include "Sandbox/ECS/System.h"

namespace Sandbox
{
	class ColliderRenderDebugSystem : public System
	{
	public:
		ColliderRenderDebugSystem();
		void OnStart() override;
		void OnRender() override;
		int GetUsedMethod() override;
		void OnRemove() override;

		void OnAddBody(ComponentSignal signal);
	private:
		uint32_t m_debugLayer;
	};
}

