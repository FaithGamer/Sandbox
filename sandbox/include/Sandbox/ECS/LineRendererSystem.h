#pragma once

#include "Sandbox/ECS/System.h"

namespace Sandbox
{
	class LineRendererSystem : public System
	{
	public:
		LineRendererSystem();
		void OnRender() override;
		int GetUsedMethod() override;
	};
}
