#include "pch.h"
#include "ColonistSystem.h"
#include "Systems/ZisYSystem.h"

ColonistSystem::ColonistSystem()
{

	ListenAddComponent<ColonistPhysics>(&ColonistSystem::OnAddColonistPhysics);
	ListenAddComponent<ColonistBrain>(&ColonistSystem::OnAddColonistBrain);

	//Fetch colonist settings
	settings = ColonistSettings(); // for now default, later will be constructed from config file
}

void ColonistSystem::OnStart()
{
	Delegate<void> aiUpdate(&ColonistSystem::AIUpdate, this);
	m_aiTask = makesptr<Task<void>>(aiUpdate);
	m_aiThread.QueueTask(m_aiTask);
	m_aiThread.StartThread();
}

void ColonistSystem::OnUpdate(Time delta)
{

	//When the thread that's working on colonist is not processing
	if (!m_aiThread.HaveTask())
	{
		//This is the time to delete and create new colonists
		SyncPoint();
		//Then keep the thread running, one task at a time.
		m_aiThread.QueueTask(m_aiTask);
	}

	//Interpolate colonist position
	ForeachEntities<ColonistPhysics, Transform>([&](Entity entity, ColonistPhysics& physics, Transform& transform)
		{
			if (physics.dead)
				return;

			physics.interpolationTime = Math::Max(0.f, physics.interpolationTime - (float)delta);
			float t = 1 - physics.interpolationTime / (float)Time::FixedDelta();
			Vec3f position = Math::Lerp(physics.prevPosition, physics.nextPosition, t);
			transform.SetPosition(position);

			if (Random::Range(0.f, 1.f) > 0.98f)
			{
				DestroyColonist(entity);
				LOG_INFO("Destroy");
			}
			if (Random::Range(0.f, 1.f) > 0.98f)
			{
				CreateColonist(ColonistInit(Vec2f(0, 0)));
				LOG_INFO("Create");
			}
		});
}

void ColonistSystem::OnFixedUpdate(Time delta)
{
	Bitmask wallMask = Physics::GetLayerMask("Walls", "Scent");
	float hitboxRadius = 0.2f;
	float margin = 0.01f;

	//Colonist movement and collisions
	ForeachComponents<ColonistPhysics, Transform>([&](ColonistPhysics& physics, Transform& transform)
		{
			if (physics.dead)
				return;

			MoveAndCollide(physics, transform, wallMask, (float)delta, margin, hitboxRadius);
		});
}

int ColonistSystem::GetUsedMethod()
{
	return System::Method::Update | System::Method::FixedUpdate;
}

void ColonistSystem::OnAddColonistPhysics(ComponentSignal signal)
{
	auto entity = Entity(signal.entity);
	auto physics = entity.GetComponent<ColonistPhysics>();
	physics->currentAngle = Random::Range(0.f, 360.f);
	physics->nextPosition = entity.GetComponent<Transform>()->GetPosition();
	physics->prevPosition = entity.GetComponent<Transform>()->GetPosition();
}

void ColonistSystem::OnAddColonistBrain(ComponentSignal signal)
{
	//Initialize colonist brain
}

void ColonistSystem::AIUpdate()
{
	ForeachComponents<ColonistBrain, Transform>([](ColonistBrain& brain, Transform& transform)
		{
			brain.lastFollowedScent = EntityId(Random::Range(0, 199));
		});

}

void ColonistSystem::DestroyColonist(Entity colonist)
{
	colonist.GetComponent<ColonistPhysics>()->dead = true;
	//to do: I'm pretty sure we can safely delete SpriteRenderer
	m_destroy.emplace_back(colonist);
}

void ColonistSystem::CreateColonist(const ColonistInit& init)
{
	m_create.emplace_back(init);
}

void ColonistSystem::SyncPoint()
{
	for (int i = 0; i < m_destroy.size(); i++)
	{
		m_destroy[i].Destroy();
	}
	for (int i = 0; i < m_create.size(); i++)
	{
		InstanceColonist(m_create[i]);
	}
	m_destroy.clear();
	m_create.clear();
}

void ColonistSystem::MoveAndCollide(
	ColonistPhysics& physics,
	Transform& transform,
	const Bitmask wallMask,
	const float delta,
	const float margin,
	const float hitboxRadius)
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

	physics.prevPosition = physics.nextPosition;
	physics.nextPosition = offset + position;
	physics.interpolationTime = (float)delta;
}

void ColonistSystem::InstanceColonist(const ColonistInit& init)
{

	//Create entity
	Entity colonist = Entity::Create();
	colonist.AddComponent<Transform>();

	//Create render 
	SpriteRender* render = colonist.AddComponent<SpriteRender>();
	render->SetSprite(Assets::Get<Sprite>("Colonists.png_0_2").Ptr()); //todo, optimize by having Prefab as a class and holding on the assets it needs

	//Colonist components
	colonist.AddComponent<ColonistBrain>();
	colonist.AddComponent<ColonistPhysics>();

	//Sprite render ordering
	colonist.AddComponent<ZisY>();
}

