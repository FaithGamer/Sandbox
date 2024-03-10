#include "pch.h"
#include "ColonistSystem.h"
#include "Components/Colonist.h"

ColonistSystem::ColonistSystem()
{

	ListenAddComponent<ColonistPhysics>(&ColonistSystem::OnAddColonistPhysics);
	ListenAddComponent<ColonistBrain>(&ColonistSystem::OnAddColonistBrain);

	//Fetch colonist settings
	settings = ColonistSettings(); // for now default, later will be constructed from config file
}

void ColonistSystem::OnStart()
{
	
}

void ColonistSystem::OnUpdate(Time delta)
{
	Bitmask wallMask = Physics::GetLayerMask("Walls");
	float hitboxRadius = 0.2f;
	ForeachComponents<ColonistPhysics, Transform>([&](ColonistPhysics& physics, Transform& transform)
		{
			//Move, collide and reflect velocity
			
			//Acceleration
			physics.speed += settings.acceleration * (float)delta;
			auto direction = Math::AngleToVec(physics.currentAngle);
			Vec3f position = transform.GetPosition();

			//Velocity
			Vec2f offset = 0;
			physics.velocity = physics.speed * direction;
			offset = direction * physics.speed * (float)delta;

			//Collision
			Vec2f radius = offset.Normalized() * hitboxRadius;
			RaycastResult raycast;
			Physics::RaycastClosest(raycast, position, (Vec2f)position + offset + radius, wallMask);

			if (raycast.hit)
			{
				offset = raycast.distance * offset.Normalized();

			}

			transform.Move(offset);
		});
}

int ColonistSystem::GetUsedMethod()
{
	return 0;
}

void ColonistSystem::OnAddColonistPhysics(ComponentSignal signal)
{
	//Initialize colonist physics
}

void ColonistSystem::OnAddColonistBrain(ComponentSignal signal)
{
	//Initialize colonist brain
}
