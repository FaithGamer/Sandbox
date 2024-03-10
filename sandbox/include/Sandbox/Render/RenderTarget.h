#pragma once
#include "Sandbox/Core/Vec.h"

namespace Sandbox
{
	/// @brief Interface for anything that can be drawed in
	class RenderTarget
	{
	public:
		virtual ~RenderTarget() {};
		virtual void SetSize(Vec2u size) = 0;
		virtual void Bind() = 0;
		virtual void Clear() = 0;
	};
}