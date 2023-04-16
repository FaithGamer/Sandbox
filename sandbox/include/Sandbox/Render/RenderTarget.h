#pragma once

namespace Sandbox
{
	class RenderTarget
	{
	public:
		virtual ~RenderTarget();
		virtual void Bind() = 0;
		virtual void Clear() = 0;
	};
}