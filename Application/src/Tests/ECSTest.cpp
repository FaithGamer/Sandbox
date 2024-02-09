#include "pch.h"
#include "Sandbox/Engine.h"
#include "Sandbox/ECS/World.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/ECS/System.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Log.h"


struct compa
{
	int b;
};

struct compb
{
	int a;
};


struct CompC
{
	int c;
};

using namespace Sandbox;

class EcsTest : public System
{
public:
	EcsTest(World* world)
	{
		m_world = world;
	}

	void OnStart() override
	{
		ListenAddComponent<CompC>(&EcsTest::OnAddCompC);
		ListenRemoveComponent<CompC>(&EcsTest::OnRemoveCompC);
	}

	void OnUpdate(Time time) override
	{
		timer += time;
		if (timer > 5)
		{
			StopListenAddComponent<CompC>();
			StopListenRemoveComponent<CompC>();
		}
		ForeachEntities(m_world, &EcsTest::MyFunction);
		ForeachEntities<compa, compb>(&EcsTest::MyFunctionEntity);
	}

	void MyFunction(Entity entity, compa& a, compb& b)
	{

	}

	void OnAddCompC(ComponentSignal signal)
	{
		std::cout << "compc added" << std::endl;
	}

	void OnRemoveCompC(ComponentSignal signal)
	{
		std::cout << "compc removed" << std::endl;
	}

	void MyFunctionEntity(Entity entity, compa& a, compb& b)
	{
		//std::cout << "MyFunctionEntity: "<< (int)entity.GetId() << "compa: "<< a.b++ << std::endl;
		entity.AddComponent<CompC>();
		entity.RemoveComponent<CompC>();
	}

	World* m_world;
	Time timer;
};


void ECSTest()
{
	Sandbox::Engine::Init();

	auto world = Sandbox::Systems::CreateWorld("MyWorld");

	Sandbox::Systems::Push<EcsTest>(world);


	Entity entity = Entity::Create();
	Entity entityH = entity;

	LOG_INFO("Entity valid: " + std::to_string(entityH.Valid()));

	entity.Destroy();

	LOG_INFO("Entity valid: " + std::to_string(entityH.Valid()));

	entity.AddComponent<compa>();
	entity.AddComponent<compb>();
	//can we create entity and add component when iterating view ?


	Sandbox::Engine::Launch();
}