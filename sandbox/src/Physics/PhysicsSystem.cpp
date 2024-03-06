#include "pch.h"
#include "Sandbox/Physics/PhysicsSystem.h"
#include "Sandbox/Physics/Body.h"

namespace Sandbox
{
	void PhysicsSystem::OnStart()
	{
		ListenAddComponent<Body>(&OnAddBody);
	}
	void PhysicsSystem::OnAddBody(ComponentSignal signal)
	{
		//Bind the body with it's entity id
		Entity(signal.entity).GetComponent<Body>()->userData = Collider::UserData(signal.entity);
	}
}