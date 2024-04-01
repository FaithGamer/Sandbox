#pragma once

#include "Sandbox/ECS/System.h"

namespace Sandbox
{
	class SpriteRenderSystem : public System
	{
	public:
		SpriteRenderSystem();
		/// @brief Sort entities on the Y axis for a clean 2D rendering of superposed semi transparent fragment
		/// Disable only if you need the extra performance boost
		/// @param sort true by default
		void YSort(bool sort);
		void OnRender() override;
		int GetUsedMethod() override;
	private:
		bool m_ySort;
	};
}

