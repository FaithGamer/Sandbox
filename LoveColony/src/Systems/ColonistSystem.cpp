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
	ForeachComponents<ColonistPhysics, Transform>([&](ColonistPhysics& physics, Transform& transform)
		{
			if (physics.dead)
				return;

			physics.interpolationTime = Math::Max(0.f, physics.interpolationTime - (float)delta);
			float t = 1 - physics.interpolationTime / (float)Time::FixedDelta();
			Vec3f position = Math::Lerp(physics.prevPosition, physics.nextPosition, t);
			transform.SetPosition(position);

		});
}

void ColonistSystem::OnFixedUpdate(Time delta)
{

	//Every physics queries happens here

	Bitmask wallMask = Physics::GetLayerMask("Walls");
	Bitmask scentMask = Physics::GetLayerMask("Scent");
	float hitboxRadius = 0.2f;
	float margin = 0.01f;


	ForeachComponents<ColonistPhysics, Transform>([&](ColonistPhysics& physics, Transform& transform)
		{
			if (physics.dead)
				return;

			//Colonist movement and collisions
			MoveAndCollide(physics, transform, wallMask, (float)delta, margin, hitboxRadius);

			//Scent detection
			std::vector<OverlapResult> overlaps;
			Physics::CircleOverlap(overlaps, transform.GetPosition(), 1.f, scentMask);
			for (int i = 0; i < overlaps.size(); i++)
			{
				auto var = overlaps[i].entityId;
			}
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

	ForeachComponents <ColonistBrain, ColonistPhysics>([&](ColonistBrain& brain, ColonistPhysics& physics)
		{
			Steering(physics, brain);

			
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

void ColonistSystem::Steering(ColonistPhysics& physics, ColonistBrain& brain)
{
	float delta = (float)Time::Delta();

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
	render->SetLayer(Renderer2D::GetLayerId("Map"));
	//Colonist components
	colonist.AddComponent<ColonistBrain>();
	colonist.AddComponent<ColonistPhysics>();

	//Sprite render ordering
	colonist.AddComponent<ZisY>();
}

