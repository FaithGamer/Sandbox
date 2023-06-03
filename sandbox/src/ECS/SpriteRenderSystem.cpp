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
				if (sprite.needUpdateRenderBatch)
				{
					sprite.renderBatch = renderer->GetBatchId(sprite.GetLayer(), sprite.GetShader(), nullptr);
					sprite.needUpdateRenderBatch = false;
				}
				renderer->DrawSprite(transform, sprite, sprite.renderBatch);
			});
	}

	int SpriteRenderSystem::GetUsedMethod()
	{
		return System::Method::Render;
	}
}