#include "pch.h"
#include "Sandbox/ECS/SpriteRenderSystem.h"

#include "Sandbox/Render.h"

namespace Sandbox
{
	SpriteRenderSystem::SpriteRenderSystem()
	{
		SetPriority(10000);
	}

	void SpriteRenderSystem::OnRender()
	{
		sptr<Renderer2D> renderer = Renderer2D::Instance();

		ForEachComponent<Sprite, Transform>([renderer](Sprite& sprite, Transform& transform)
			{
				if (sprite.needUpdateRenderPipeline)
				{
					sprite.renderPipeline = renderer->GetPipeline(sprite.GetLayer(), sprite.GetShader(), nullptr);
					sprite.needUpdateRenderPipeline = false;
				}
				renderer->DrawSprite(transform, sprite, sprite.renderPipeline);
			});
	}

	int SpriteRenderSystem::GetUsedMethod()
	{
		return System::Method::Render;
	}
}