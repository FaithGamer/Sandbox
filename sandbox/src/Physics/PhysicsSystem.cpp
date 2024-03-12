#include "pch.h"
#include "Sandbox/Physics/PhysicsSystem.h"
#include "Sandbox/Physics/Body.h"
#include "Sandbox/Render/Transform.h"

namespace Sandbox
{
	PhysicsSystem::PhysicsSystem()
	{
		SetPriority(99999);
		ListenAddComponent<KinematicBody>(&PhysicsSystem::OnAddKinematicBody);
		ListenAddComponent<StaticBody>(&PhysicsSystem::OnAddStaticBody);
	}
	void PhysicsSystem::OnStart()
	{
	}
	void PhysicsSystem::OnAddKinematicBody(ComponentSignal signal)
	{
		//Bind the body with it's entity id
		Entity(signal.entity).GetComponent<KinematicBody>()->userData = Collider::UserData(signal.entity);
	}
	void PhysicsSystem::OnAddStaticBody(ComponentSignal signal)
	{
		//Bind the body with it's entity id
		Entity entity(signal.entity);
		auto body = entity.GetComponent<StaticBody>();
		body->userData = Collider::UserData(signal.entity);

	}
	void PhysicsSystem::OnUpdate(Time deltaTime)
	{
		ForeachComponents<KinematicBody, Transform>([](KinematicBody& body, Transform& transform)
			{
				body.UpdateTransform(transform.GetPosition(), transform.GetRotation().z);
			});
	}
	int PhysicsSystem::GetUsedMethod()
	{
		return System::Method::Update;
	}

}