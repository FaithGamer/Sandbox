#include "pch.h"
#include "Sandbox/ECS/ColliderRenderDebugSystem.h"
#include "Sandbox/Physics/Collider.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Physics/Body.h"

namespace Sandbox
{
	void AddColliderRender(Entity bodyEntt)
	{
		auto body = bodyEntt.GetComponent<Body>();
		auto colliders = body->GetColliders();
		for (int i = 0; i < colliders->size(); i++)
		{
			Entity wireEntt = Entity::Create();
			wireEntt.AddComponent<ColliderRender>(&*(*colliders)[i]);
			wireEntt.AddComponent<Transform>();
			bodyEntt.AddChild(wireEntt);
		}
	}

	ColliderRenderDebugSystem::ColliderRenderDebugSystem() : m_debugLayer(0)
	{
		SetPriority(-9999);
	}

	void ColliderRenderDebugSystem::OnStart()
	{
		//OnStart is called after engine launch.
		m_debugLayer = Renderer2D::GetLayerId("DebugLayer");

		//Will add a collider render for each new body
		ListenAddComponent<Body>(&ColliderRenderDebugSystem::OnAddBody);

		//Create ColliderRender for every body
		ForeachEntities<Body>([](Entity entity, Body& body)
			{
				AddColliderRender(entity);
			});
	}

	void ColliderRenderDebugSystem::OnRender()
	{
		sptr<Renderer2D> renderer = Renderer2D::Instance();

		ForeachComponents<ColliderRender, Transform>([&](ColliderRender& collider, Transform& transform)
			{
				renderer->DrawWire(*collider.wire, transform, m_debugLayer);
			});
	}

	int ColliderRenderDebugSystem::GetUsedMethod()
	{
		return System::Method::Render;
	}

	void ColliderRenderDebugSystem::OnRemove()
	{
		StopListenAddComponent<Body>();
		ForeachEntities<ColliderRender>([](Entity entity, ColliderRender& collider)
			{
				entity.Destroy();
			});
	}

	void ColliderRenderDebugSystem::OnAddBody(ComponentSignal signal)
	{
		AddColliderRender(Entity(signal.entity));
	}
}