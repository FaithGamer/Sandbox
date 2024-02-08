#include "pch.h"
#include "RaycastTest.h"

#include "Sandbox/ECS.h"
#include "Sandbox/Render.h"
#include "box2d/box2d.h"
#include "Sandbox/Engine.h"

using namespace Sandbox;

class RaycastCallback : public b2RayCastCallback
{
public:
	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
	{
		hit = true;
		_point = point;
		_normal = normal;

		return fraction;
	}
	bool hit = false;
	Vec2f _point;
	Vec2f _normal;
};
void RaycastTest()
{
	Engine::Init();

	b2World world(Vec2f(0, 0));

	b2BodyDef bodyDefault;

	auto body1 = world.CreateBody(&bodyDefault);
	//auto body2 = world.CreateBody(&bodyDefault);

	b2PolygonShape shape;
	shape.SetAsBox(10, 10);

	b2FixtureDef fixtureDefault;

	fixtureDefault.shape = &shape;

	body1->CreateFixture(&fixtureDefault);

	body1->DestroyFixture

	RaycastCallback raycast;

	world.RayCast(&raycast, Vec2f(100, 0), Vec2f(-100, 0));

	LOG_INFO("Raycast 1 hit? " + std::to_string(raycast.hit));

	raycast.hit = false;
	world.RayCast(&raycast, Vec2f(-100, 0), Vec2f(-200, 0));

	LOG_INFO("Raycast 2 hit? " + std::to_string(raycast.hit));

}