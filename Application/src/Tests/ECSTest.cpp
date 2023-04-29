#include "pch.h"
#include "Sandbox/Engine.h"
#include "Sandbox/ECS/GameWorld.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/ECS/System.h"
#include "Sandbox/ECS/Systems.h"


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
	EcsTest(GameWorld* world)
	{
		m_world = world;
	}

	void OnStart() override
	{
		m_world->ListenOnAddComponent<CompC>(&EcsTest::OnAddCompC, this);
		m_world->ListenOnRemoveComponent<CompC>(&EcsTest::OnRemoveCompC, this);
	}

	void OnUpdate(Time time) override
	{
		ForEachComponent(m_world, &EcsTest::MyFunction);
		ForEachComponent(m_world, &EcsTest::MyFunctionEntity);
	}

	void MyFunction(compa& a, compb& b)
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

	void MyFunctionEntity(Entity& entity, compa& a, compb& b)
	{
		//std::cout << "MyFunctionEntity: "<< (int)entity.GetId() << "compa: "<< a.b++ << std::endl;
		entity.AddComponent<CompC>();
		entity.RemoveComponent<CompC>();
	}

	GameWorld* m_world;
};


void ECSTest()
{
	Sandbox::Engine::Init();

	auto world = Sandbox::Systems::CreateGameWorld("MyWorld");

	Sandbox::Systems::Push<EcsTest>(world);


	Entity* entity = world->CreateEntity();
	entity->AddComponent<compa>();
	entity->AddComponent<compb>();
	//can we create entity and add component when iterating view ?


	Sandbox::Engine::Launch();
}