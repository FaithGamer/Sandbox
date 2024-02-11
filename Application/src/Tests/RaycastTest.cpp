#include "pch.h"
#include "RaycastTest.h"

#include "Sandbox/ECS.h"
#include "Sandbox/Render.h"
#include "box2d/box2d.h"
#include "Sandbox/Engine.h"
#include "Sandbox/Physics/Raycast.h"
#include "Sandbox/Physics/Bitmask.h"


using namespace Sandbox;




void RaycastTest()
{
	Engine::Init();


	///__ b2 raycast

	b2World world(Vec2f(0, 0));

	b2BodyDef bodyDefault;
	auto body1 = world.CreateBody(&bodyDefault);

	b2PolygonShape shape;
	shape.SetAsBox(10, 10);
	b2FixtureDef fixtureDefault;
	fixtureDefault.shape = &shape;
	body1->CreateFixture(&fixtureDefault);

	RaycastCallbackClosest raycast1;
	world.RayCast(&raycast1, Vec2f(100, 0), Vec2f(-100, 0));
	LOG_INFO("Raycast 1 hit? " + std::to_string(raycast1.result));

	body1->SetTransform(Vec2f(1000, 1000), 0);

	RaycastCallbackClosest raycast2;
	world.RayCast(&raycast2, Vec2f(100, 0), Vec2f(-100, 0));
	LOG_INFO("Raycast 2 result? " + std::to_string(raycast2.result));

	///_____ filter test


	Filter filter;

	filter.AddFlag("Banane");
	filter.AddFlag("Cornichon");
	filter.AddFlag("Poire");

	Bitmask mask1 = filter.GetMask("Banane");
	Bitmask mask2 = filter.GetMask("Cornichon");
	Bitmask mask3 = filter.GetMask("Poire");

	std::cout << "Is the flag setting working ?" << std::endl;
	std::cout << "Banane mask - " << mask1.flags << std::endl;
	std::cout << "Cornichon mask - " << mask2.flags << std::endl;
	std::cout << "Poire mask - " << mask3.flags << std::endl;

	bool contain = filter.BitmaskContains(mask1, "Banane");
	std::cout << "Banane mask contains Banane ? " << contain << std::endl;

	contain = filter.BitmaskContains(mask3, "Poire");
	std::cout << "Poire mask contains Poire ? " << contain << std::endl;

	contain = filter.BitmaskContains(mask1, "Cornichon");
	std::cout << "Banane mask contains Cornichon ? " << contain << std::endl;

	Bitmask cornipoire = filter.GetMask("Cornichon", "Poire");

	contain = filter.BitmaskContains(cornipoire, "Cornichon");
	std::cout << "Cornipoire mask contains Cornichon ? " << contain << std::endl;
	contain = filter.BitmaskContains(cornipoire, "Poire");
	std::cout << "Cornipoire mask contains Poire ? " << contain << std::endl;
	contain = filter.BitmaskContains(cornipoire, "Banane");
	std::cout << "Cornipoire mask contains Banane ? " << contain << std::endl;

	std::cout << "Cornipoire mask - " << cornipoire.flags << std::endl;


	///___ physics test




}