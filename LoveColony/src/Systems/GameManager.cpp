#include "pch.h"
#include "GameManager.h"
#include "Prefab.h"
#include "Components/Colonist.h"
#include "Components/Scent.h"
#include "Components/POI.h"
#include "Systems/ScentSystem.h"


GameManager::GameManager()
{
}

void GameManager::OnStart()
{
	StartGame();
	m_aiThread.StartThread();
	m_physicsThread.StartThread();
	Systems::Get<ScentSystem>()->DebugShowScent(true);
}

void GameManager::OnUpdate(Time delta)
{

}

void GameManager::OnFixedUpdate(Time delta)
{
	m_threadAccumulator += (float)delta;
	if (!m_aiThread.HaveTask() && !m_physicsThread.HaveTask())
	{
		//Create and destroy entity when only the main thread is running
		for (auto it = m_entityDestroy.begin(); it != m_entityDestroy.end(); it++)
		{
			Entity(*it).Destroy();
		}
		for (int i = 0; i < m_entityCreate.size(); i++)
		{
			m_entityCreate[i]->CreateEntity();
		}
		

		m_entityDestroy.clear();
		m_entityCreate.clear();

		//Send signal so thread can have tasks queued
		LOG_INFO("thread accumulator: " + std::to_string(m_threadAccumulator));
		threadSyncSignal.SendSignal(ThreadSyncSignal(m_threadAccumulator));
		m_threadAccumulator = 0;
	}


}

void GameManager::StartGame()
{
	CreateMap();
	CreateEntities();
}

void GameManager::DestroyEntity(Entity entity)
{
	std::lock_guard lock(m_destroyMutex);
	m_entityDestroy.insert(entity.GetId());
}

void GameManager::CreateEntity(sptr<Init> init)
{
	std::lock_guard lock(m_createMutex);
	m_entityCreate.emplace_back(init);
}

void GameManager::QueuePhysicsTask(sptr<OpaqueTask> task)
{
	m_physicsThread.QueueTask(task);
}

void GameManager::QueueAITask(sptr<OpaqueTask> task)
{
	m_aiThread.QueueTask(task);
}

Entity CreateBox(Vec2f position, Vec2f dimensions)
{
	Entity box = Entity::Create();
	auto transform = box.AddComponent<Transform>();
	transform->SetPosition(position);
	auto body = box.AddComponent<StaticBody>(position, Physics::GetLayerMask("Walls"));
	body->SetLayerMask(Bitmask(333));
	body->AddCollider(makesptr<Box2D>(dimensions));

	return box;

}
int mapHeight = 20;
int mapWidth = 50;
Vec2f GridToWorld(int x, int y)
{
	return Vec2f(
		x - mapWidth * 0.5f,
		y - mapHeight * 0.5f
	);
}
void GameManager::CreateMap()
{
	//Create world edges
	CreateBox(Vec2f(25, 0), Vec2f(4, 40));
	CreateBox(Vec2f(-25, 0), Vec2f(4, 40));
	CreateBox(Vec2f(0, 14), Vec2f(50, 4));
	CreateBox(Vec2f(0, -14), Vec2f(50, 4));


	//Create walls

	float density = 0.3f;

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			float noise = Noise::Noise2D((float)x, (float)y, 0.4);
			if (noise < density)
			{
				//Entity wall = Prefab::Wall(GridToWorld(x, y));
			}
		}
	}
}

void GameManager::CreateEntities()
{
	//Create test entities
	for (int i = 0; i < 39; i++)
	{
		auto init = makesptr<ColonistInit>();
		init->position = Vec2f(Random::Range(-25.f, 25.f), Random::Range(-10.f, 10.f));
		CreateEntity(init);
	}

	//Create test food
	for (int i = 0; i < 2; i++)
	{
		auto init = makesptr<POIInit>();
		init->position = Vec2f(Random::Range(-25.f, 25.f), Random::Range(-11.f, 11.f));
		init->type = POIType::Food;
		init->hitboxRadius = 1.f;
		init->startCount = 4000;
		CreateEntity(init);
	}

	//Create test shelters
	for (int i = 0; i < 2; i++)
	{
		auto init = makesptr<POIInit>();
		init->position = Vec2f(Random::Range(-25.f, 25.f), Random::Range(-11.f, 11.f));
		init->type = POIType::Shelter;
		init->hitboxRadius = 1.f;
		init->startCount = 0;
		CreateEntity(init);
	}

	/*for (int i = 0; i < 4000; i++)
	{
		auto pos = Vec2f(Random::Range(-25.f, 25.f), Random::Range(-10.f, 10.f));
		auto init = makesptr <ScentInit>();
		init->type = Scent::Type::Food;
		init->position = pos;
		init->poiDistance = 0;
		CreateEntity(init);
	}*/

	//trying to see if the small scal problem will appear when creating entities before t hreading
	/*for (int i = 0; i < 10000; i++)
	{
		if (Random::Range(0.f, 1.f) >= 0.5f)
		{
			auto init = makesptr<ColonistInit>();
			init->position = Vec2f(Random::Range(-25.f, 25.f), Random::Range(-10.f, 10.f));
			CreateEntity(init);
		}
		else
		{
			auto pos = Vec2f(Random::Range(-25.f, 25.f), Random::Range(-10.f, 10.f));
			auto init = makesptr <ScentInit>();
			init->type = Scent::Type::Food;
			init->position = pos;
			init->poiDistance = 0;
			CreateEntity(init);
		}
	}*/
}