#pragma once

namespace Sandbox
{
	class RenderTarget
	{
	public:
		virtual ~RenderTarget() {};
		virtual void SetSize(Vec2u size) = 0;
		virtual void Bind() = 0;
		virtual void Clear() = 0;
	};
}