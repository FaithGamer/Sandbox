#pragma once

#include "Sandbox/ECS/System.h"

namespace Sandbox
{
	class SpriteRenderSystem : public System
	{
	public:
		SpriteRenderSystem();
		void OnRender() override;
		int GetUsedMethod() override;
	};
}

