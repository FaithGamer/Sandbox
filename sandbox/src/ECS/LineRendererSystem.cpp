#include "pch.h"
#include "Sandbox/ECS/LineRendererSystem.h"
#include "Sandbox/Render/LineRenderer.h"
#include "Sandbox/Render/Renderer2D.h"

namespace Sandbox
{
	LineRendererSystem::LineRendererSystem()
	{
		SetPriority(9999);
	}

	void LineRendererSystem::OnRender()
	{
		sptr<Renderer2D> renderer = Renderer2D::Instance();

		ForEachComponent<LineRenderer, Transform>([renderer](LineRenderer& line, Transform& transform)
			{
				renderer->DrawLine(line, transform, line.GetLayer());
			});

	}

	int LineRendererSystem::GetUsedMethod()
	{
		return System::Method::Render;
	}
}