#pragma once

#include "Sandbox/ECS/System.h"

namespace Sandbox
{
	class WireRenderSystem : public System
	{
	public:
		WireRenderSystem();
		void OnRender() override;
		int GetUsedMethod() override;
	};
}

#pragma once
