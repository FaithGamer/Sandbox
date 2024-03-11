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
	/*Bitmask wallMask = Physics::GetLayerMask("Walls", "Scent");
	float hitboxRadius = 0.2f;
	float margin = 0.01f;*/
	ForeachComponents<ColonistPhysics, Transform>([&](ColonistPhysics& physics, Transform& transform)
		{
			physics.interpolationTime = Math::Max(0.f, physics.interpolationTime - (float)delta);
			float t = 1 - physics.interpolationTime / (float)Time::FixedDelta();
			Vec3f position = Math::Lerp(physics.prevPosition, physics.nextPosition, t);
			transform.SetPosition(position);
		});


}

void ColonistSystem::OnFixedUpdate(Time delta)
{
	Bitmask wallMask = Physics::GetLayerMask("Walls", "Scent");
	float hitboxRadius = 0.2f;
	float margin = 0.01f;

	ForeachComponents<ColonistPhysics, Transform>([&](ColonistPhysics& physics, Transform& transform)
		{
			//Move, collide and reflect velocity
			Vec2f position = (Vec2f)transform.GetPosition();
			Vec2f direction = Math::AngleToVec(physics.currentAngle);

			//Acceleration
			physics.speed += settings.acceleration * (float)delta;
			physics.speed = Math::Clamp(physics.speed, 0, settings.maxSpeed);

			//Velocity
			Vec2f offset = 0;
			physics.velocity = physics.speed * direction;
			offset = direction * physics.speed * (float)delta;

			//Collision
			Vec2f radius = offset.Normalized() * (hitboxRadius + margin);
			RaycastResult raycast;
			Physics::RaycastClosest(raycast, position, offset + position + radius, wallMask);

			if (raycast.hit)
			{
				offset = (raycast.distance - margin - hitboxRadius) * offset.Normalized();

				direction = direction.Reflected(raycast.normal);
				physics.currentAngle = Math::VecToAngle(direction);
				physics.velocity = physics.speed * direction;
			}

			physics.prevPosition = position;
			physics.nextPosition = offset + position;
			physics.interpolationTime = (float)delta;
			//transform.Move(offset);
		});
}

int ColonistSystem::GetUsedMethod()
{
	return System::Method::Update | System::Method::FixedUpdate;
}

void ColonistSystem::OnAddColonistPhysics(ComponentSignal signal)
{
	Entity(signal.entity).GetComponent<ColonistPhysics>()->currentAngle = Random::Range(0.f, 360.f);
}

void ColonistSystem::OnAddColonistBrain(ComponentSignal signal)
{
	//Initialize colonist brain
}
