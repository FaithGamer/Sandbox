#include "pch.h"
#include "Sandbox/ECS/SpriteRenderSystem.h"

#include "Sandbox/Render.h"

namespace Sandbox
{
	struct OrderedSpriteTransform
	{
		SpriteRender* sprite;
		Transform* transform;
		float z;
		bool operator<(const OrderedSpriteTransform rhs) const
		{
			return z > rhs.z;
		}
	};

	SpriteRenderSystem::SpriteRenderSystem() : m_zSort(true)
	{
		SetPriority(10000);
	}

	void SpriteRenderSystem::SetZSort(bool sort)
	{
		m_zSort = sort;
	}
	bool SpriteRenderSystem::GetZSort()
	{
		return m_zSort;
	}
	void SpriteRenderSystem::OnRender()
	{
		sptr<Renderer2D> renderer = Renderer2D::Instance();

		if (!m_zSort)
		{
			ForeachComponents<SpriteRender, Transform>([renderer](SpriteRender& sprite, Transform& transform)
				{
					if (sprite.needUpdateRenderBatch)
					{
						sprite.renderBatch = renderer->GetBatchId(sprite.GetLayer(), sprite.GetShader(), nullptr);
						sprite.needUpdateRenderBatch = false;
					}
					renderer->DrawSprite(transform, sprite, sprite.renderBatch);
				});
		}
		else
		{
			std::list<OrderedSpriteTransform> ordered;

			ForeachComponents<SpriteRender, Transform>([&ordered, renderer](SpriteRender& sprite, Transform& transform)
				{
					auto ord = OrderedSpriteTransform(&sprite, &transform, transform.GetPosition().z);
					ordered.emplace_back(ord);
				});

			ordered.sort();

			for (auto& sprite : ordered)
			{
				if (sprite.sprite->needUpdateRenderBatch)
				{
					sprite.sprite->renderBatch = renderer->GetBatchId(sprite.sprite->GetLayer(), sprite.sprite->GetShader(), nullptr);
					sprite.sprite->needUpdateRenderBatch = false;
				}
				renderer->DrawSprite(*sprite.transform, *sprite.sprite, sprite.sprite->renderBatch);
			}
		}
	}

	int SpriteRenderSystem::GetUsedMethod()
	{
		return System::Method::Render;
	}
}