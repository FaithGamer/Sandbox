#include "pch.h"
#include "ColonistSystem.h"
#include "Systems/ZisYSystem.h"
#include "ScentSystem.h"

ColonistSystem::ColonistSystem()
{
	ListenAddComponent<ColonistPhysics>(&ColonistSystem::OnAddColonistPhysics);
	ListenAddComponent<ColonistBrain>(&ColonistSystem::OnAddColonistBrain);

	//Fetch colonist settings
	settings = ColonistSettings(); // for now default, later will be constructed from config file
}

void ColonistSystem::OnStart()
{
	m_scentSystem = Systems::Get<ScentSystem>();
	m_gameManager = Systems::Get<GameManager>();
	m_gameManager->threadSyncSignal.AddListener(&ColonistSystem::OnSyncThread, this);
}

void ColonistSystem::OnUpdate(Time delta)
{
	EntityId deletechance = EntityId(0);
	//Interpolate colonist position
	ForeachEntities<ColonistPhysics, Transform>([&](Entity& entity, ColonistPhysics& physics, Transform& transform)
		{
			if (physics.dead)
				return;

			physics.interpolationTimer = Math::Max(0.f, physics.interpolationTimer - (float)delta);
			float t = Math::Clamp01(1 - physics.interpolationTimer / physics.interpolationTime);
			Vec3f position = Math::Lerp(physics.prevPosition, physics.nextPosition, t);
			transform.SetPosition(position);



			/*if (Random::Range(0.f, 1.f) > 0.98f)
			{
				m_gameManager->DestroyEntity(entity);
			}*/
			deletechance = entity.GetId();

		});

	/*if (Random::Range(0.f, 1.f) > 0.9f)
	{
		auto init = makesptr<ColonistInit>();
		init->position = Vec2f(Random::Range(-25.f, 25.f), Random::Range(-10.f, 10.f));
		m_gameManager->CreateEntity(init);
	}*/

	/*if (Random::Range(0.f, 1.f) > 0.93f && deletechance != EntityId(0))
	{
		//m_gameManager->DestroyEntity(Entity(deletechance));
	}

	if (Random::Range(0.f, 1.f) > 0.01f)
	{
		auto pos = Vec2f(Random::Range(-25.f, 25.f), Random::Range(-10.f, 10.f));
		auto init = makesptr <ScentInit>();
		init->type = Scent::Type::Food;
		init->position = pos;
		init->poiDistance = 0;
		m_gameManager->CreateEntity(init);
	}*/


	
}

void ColonistSystem::OnFixedUpdate(Time delta)
{


}

int ColonistSystem::GetUsedMethod()
{
	return System::Method::Update | System::Method::FixedUpdate;
}

void ColonistSystem::OnAddColonistPhysics(ComponentSignal signal)
{
	auto entity = Entity(signal.entity);
	auto physics = entity.GetComponentNoCheck<ColonistPhysics>();
	physics->currentAngle = Random::Range(0.f, 360.f);
	physics->nextPosition = entity.GetComponentNoCheck<Transform>()->GetPosition();
	physics->prevPosition = entity.GetComponentNoCheck<Transform>()->GetPosition();
	physics->interpolationTime = 1;
}

void ColonistSystem::OnAddColonistBrain(ComponentSignal signal)
{
	//Initialize colonist brain
}

void ColonistSystem::OnSyncThread(ThreadSyncSignal signal)
{
	//Create tasks for thread
	auto aiDelegate = Delegate<void, float>(&ColonistSystem::AIUpdate, this, signal.delta);
	auto physicsDelegate = Delegate<void, float>(&ColonistSystem::PhysicsUpdate, this, signal.delta);
	m_gameManager->QueueAITask(makesptr<Task<void, float>>(aiDelegate));
	m_gameManager->QueuePhysicsTask(makesptr<Task<void, float>>(physicsDelegate));
}

void ColonistSystem::AIUpdate(float delta)
{

	ForeachComponents <ColonistBrain, ColonistPhysics>([&](ColonistBrain& brain, ColonistPhysics& physics)
		{
			Steering(physics, brain, delta);
		});
}

void ColonistSystem::PhysicsUpdate(float delta)
{
	Clock clock;
	Bitmask wallMask = Physics::GetLayerMask("Walls");
	Bitmask scentMask = Physics::GetLayerMask("Scent");
	float hitboxRadius = 0.2f;
	float margin = 0.01f;

	ForeachComponents<ColonistPhysics>([&](ColonistPhysics& physics)
		{
			if (physics.dead)
				return;

			//Colonist movement and collisions
			MoveAndCollide(physics, wallMask, delta, margin, hitboxRadius);

			//Scent detection
			std::vector<OverlapResult> overlaps;
			Physics::CircleOverlap(overlaps, physics.prevPosition, settings.sensorRadius, scentMask);
			Vec2f sensorPosition = physics.prevPosition 
				+ Vec2f(Math::AngleToVec(physics.currentAngle)).Normalized()
				* settings.sensorDistance;

			//Every scent in a radius around the colonist
			for (int i = 0; i < overlaps.size(); i++)
			{
				Entity scentEntity = Entity(overlaps[i].entityId);
				auto scent = scentEntity.GetComponentNoCheck<Scent>();

				//Check if scent is interesting
				if (ScentMatch(physics.state, scent->type))
				{
					auto scentTransform = scentEntity.GetComponentNoCheck<Transform>();
					Vec2f sensorDirection = (Vec2f)scentTransform->GetPosition() - sensorPosition;

					//Check if scent is within the cone of detection
					float angle = Math::Abs(physics.velocity.Normalized().Angle(sensorDirection.Normalized()));
					if (angle < settings.sensorAngle / 2)
					{
						//Scent is followed
						physics.lastFollowedScent = overlaps[i].entityId;

						//Set the scent sprite white
						auto sprite = scentEntity.GetComponent<SpriteRender>();
						if (sprite != nullptr)
						{
							sprite->color = Vec4f(1);
						}
					}
				}
			}

			//Scent dropping
			physics.distanceFromLastScentDrop += physics.prevPosition.Distance(physics.nextPosition);
			if (physics.distanceFromLastScentDrop >= settings.scentDistance)
			{
				auto scent = makesptr<ScentInit>();
				scent->position = physics.prevPosition;
				m_scentSystem->TryCreateTrackScent(scent, overlaps);
				physics.distanceFromLastScentDrop = 0;
			}
		});
	
}


void ColonistSystem::Steering(ColonistPhysics& physics, ColonistBrain& brain, float delta)
{
	//Wandering direction
	brain.wanderTimer += delta;
	if (brain.wanderTimer > brain.nextWanderTime)
	{
		brain.wanderTimer = 0;
		brain.wanderDirection = Random::Range(-1.f, 1.f);
		brain.nextWanderTime = Random::Range(settings.wanderTimeMin, settings.wanderTimeMax);
	}

	//Computing target angle (currently wander direction)
	float targetAngle = physics.currentAngle;

	//Wandering
	targetAngle = Math::MoveTowardsAngle(targetAngle, targetAngle + brain.wanderDirection * settings.wanderPower * delta, 999);
	//Steering toward target angle
	float c = Math::MoveTowardsAngle(physics.currentAngle, targetAngle, settings.steeringSpeed * settings.maxSpeed * 100 * delta);
	physics.currentAngle = c;

}

void ColonistSystem::MoveAndCollide(
	ColonistPhysics& physics,
	const Bitmask wallMask,
	const float delta,
	const float margin,
	const float hitboxRadius)
{
	//Move, collide and reflect velocity
	physics.prevPosition = physics.nextPosition;
	auto position = physics.prevPosition;
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

	
	physics.nextPosition = offset + position;
	physics.interpolationTime = (float)delta;
	physics.interpolationTimer = (float)delta;
}

bool ColonistSystem::ScentMatch(ColonistState state, Scent::Type scentType) const
{
	switch (state)
	{
	case ColonistState::SearchingFood:
		return scentType == Scent::Type::Food;
	case ColonistState::SearchingShelter:
		return scentType == Scent::Type::Shelter;
	}
	return false;
}

