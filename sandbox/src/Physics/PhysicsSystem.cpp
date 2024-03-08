#include "pch.h"
#include "Sandbox/Physics/PhysicsSystem.h"
#include "Sandbox/Physics/Body.h"
#include "Sandbox/Render/Transform.h"

namespace Sandbox
{
	PhysicsSystem::PhysicsSystem()
	{
		SetPriority(99999);
		ListenAddComponent<Body>(&PhysicsSystem::OnAddBody);
	}
	void PhysicsSystem::OnStart()
	{
	}
	void PhysicsSystem::OnAddBody(ComponentSignal signal)
	{
		//Bind the body with it's entity id
		Entity(signal.entity).GetComponent<Body>()->userData = Collider::UserData(signal.entity);
	}
	void PhysicsSystem::OnUpdate(Time deltaTime)
	{
		ForeachComponents<Body, Transform>([](Body& body, Transform& transform)
			{
				body.UpdateTransform(transform.GetWorldPosition(), transform.GetWorldRotation().z);
			});
	}
	int PhysicsSystem::GetUsedMethod()
	{
		return System::Method::Update;
	}

}