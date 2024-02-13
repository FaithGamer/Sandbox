#include "pch.h"
#include "Sandbox/ECS/WireRenderSystem.h"
#include "Sandbox/Render/WireRender.h"
#include "Sandbox/Render/Renderer2D.h"

namespace Sandbox
{
	WireRenderSystem::WireRenderSystem()
	{
		SetPriority(-9999);
	}

	void WireRenderSystem::OnRender()
	{
		sptr<Renderer2D> renderer = Renderer2D::Instance();

		ForeachComponents<WireRender, Transform>([renderer](WireRender& wire, Transform& transform)
			{
				renderer->DrawWire(wire, transform, wire.GetLayer());
			});
	}

	int WireRenderSystem::GetUsedMethod()
	{
		return System::Method::Render;
	}
}