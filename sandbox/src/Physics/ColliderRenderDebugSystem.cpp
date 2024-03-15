#include "pch.h"
#include "Sandbox/Physics/ColliderRenderDebugSystem.h"
#include "Sandbox/Physics/Collider.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Physics/Body.h"

namespace Sandbox
{
	void AddColliderRender(Entity bodyEntt, KinematicBody* body)
	{
		auto colliders = body->GetColliders();
		for (int i = 0; i < colliders->size(); i++)
		{
			Entity wireEntt = Entity::Create();
			wireEntt.AddComponent<ColliderRender>(&*(*colliders)[i]);
			wireEntt.AddComponent<Transform>();
			bodyEntt.AddChild(wireEntt);
		}
	}

	void AddColliderRender(Entity bodyEntt, StaticBody* body)
	{
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

		//Will add a collider render for each new body
		ListenAddComponent<KinematicBody>(&ColliderRenderDebugSystem::OnAddKinematicBody);
		ListenAddComponent<StaticBody>(&ColliderRenderDebugSystem::OnAddStaticBody);
	}

	void ColliderRenderDebugSystem::OnStart()
	{
		m_debugLayer = Renderer2D::GetLayerId("DebugLayer");

		//Create ColliderRender for every body
		ForeachEntities<KinematicBody>([](Entity entity, KinematicBody& body)
			{
				AddColliderRender(entity, &body);
			});
		ForeachEntities<StaticBody>([](Entity entity, StaticBody& body)
			{
				AddColliderRender(entity, &body);
			});
	}

	void ColliderRenderDebugSystem::OnUpdate(Time delta)
	{
		for (auto it = m_newKinematicBodies.begin(); it != m_newKinematicBodies.end(); it++)
		{
			Entity entity(*it);
			AddColliderRender(entity, entity.GetComponentNoCheck<KinematicBody>());
		}
		for (auto it = m_newStaticBodies.begin(); it != m_newStaticBodies.end(); it++)
		{
			Entity entity(*it);
			AddColliderRender(entity, entity.GetComponentNoCheck<StaticBody>());
		}
		m_newKinematicBodies.clear();
		m_newStaticBodies.clear();
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
		return System::Method::Render | System::Method::Update;
	}

	void ColliderRenderDebugSystem::OnRemove()
	{
		StopListenAddComponent<Body>();
		ForeachEntities<ColliderRender>([](Entity entity, ColliderRender& collider)
			{
				entity.Destroy();
			});
	}

	void ColliderRenderDebugSystem::OnAddKinematicBody(ComponentSignal signal)
	{
		m_newKinematicBodies.insert(signal.entity);
	}
	void ColliderRenderDebugSystem::OnAddStaticBody(ComponentSignal signal)
	{
		m_newStaticBodies.insert(signal.entity);
	}
}