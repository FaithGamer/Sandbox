#include "pch.h"
#include "Sandbox/Engine.h"
#include "Sandbox/System/Systems.h"
#include "Tests/ImGuiSystemTest.h"
#include "entt/entt.hpp"
#include "Sandbox/Time.h"

#include <random>
#include <iostream>

#include "Sandbox/System/System.h"
#include "Sandbox/Entity.h"


struct compa
{
	int b;
};

struct compb
{
	int a;
};

using namespace Sandbox;



class WorldTest : public System
{
public:
	WorldTest(GameWorld* world)
	{
		m_world = world;
	}

	void OnUpdate(Time time) override
	{
		std::cout << "oui" << std::endl;
		ForEachComponent(&WorldTest::MyFunction, m_world);
		ForEachComponent(&WorldTest::MyFunctionEntity, m_world);
	}

	void MyFunction(compa& a, compb& b)
	{

	}

	void MyFunctionEntity(Entity& entity, compa& a, compb& b)
	{
		std::cout << "MyFunctionEntity: "<< (int)entity.GetId() << std::endl;
	}

	GameWorld* m_world;
};

int main(int argc, char** argv)
{
	Sandbox::Engine::Init();

	auto world = Sandbox::Systems::CreateGameWorld("MyWorld");

	Sandbox::Systems::Push<ImGuiSystemTest>();
	Sandbox::Systems::Push<WorldTest>(world);
	
	Entity& entity = world->CreateEntity();
	entity.AddComponent<compa>();
	entity.AddComponent<compb>();
	//can we create entity and add component when iterating view ?
	

	Sandbox::Engine::Launch();

	return 0;
}