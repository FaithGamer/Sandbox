#include "pch.h"
#include "Sandbox/ECS/LineRendererSystem.h"
#include "Sandbox/Render/LineRenderer.h"
#include "Sandbox/Render/Renderer2D.h"

namespace Sandbox
{
	LineRendererSystem::LineRendererSystem()
	{
		SetPriority(9998);
	}

	void LineRendererSystem::OnRender()
	{


	}

	void LineRendererSystem::Render()
	{
		//Called by the Renderer directly because the depth test doesn't work for lines if they're draw call is issued before the one of quads
		//I don't know why, so this is just like this because it works.
		sptr<Renderer2D> renderer = Renderer2D::Instance();

		ForeachComponents<LineRenderer, Transform>([renderer](LineRenderer& line, Transform& transform)
			{
				renderer->DrawLine(line, transform, line.GetLayer());
			});
	}

	int LineRendererSystem::GetUsedMethod()
	{
		return 0;
	}
}