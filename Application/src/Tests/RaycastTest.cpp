#include "pch.h"
#include "RaycastTest.h"

#include "Sandbox/ECS.h"
#include "Sandbox/Render.h"
#include "box2d/box2d.h"
#include "Sandbox/Engine.h"
#include "Sandbox/Physics.h"
#include "Sandbox/Assets.h"
#include "Sandbox/Math.h"
#include "Sandbox/Render/Renderer2D.h"

using namespace Sandbox;



void Overlap(Vec2f pos, float radius)
{
	std::vector<OverlapResult> results;
	Physics::CircleOverlap(results, pos, radius);


	for (auto& result : results)
	{
		std::cout << "Circle overlap with entity #" << (unsigned int)result.entity.GetId() << std::endl;
	}
}


class CollisionTestSystem : public System
{
public:
	void OnUpdate(Time delta) override
	{
		timer += (float)delta*50;
		y = Math::Sin(timer) * 10;

		ForeachComponents<Body, Transform>([&](Body& body, Transform& trans)
			{
				body.UpdateTransform(trans.GetWorldPosition(), trans.GetRotationZAxis());
			});

		ForeachEntities<Body, SpriteRender>([&](Entity entity, Body& body, SpriteRender& sprite)
			{
				auto trans = entity.GetComponent<Transform>();
				trans->SetPosition({ -0.1, y, 0 });
				std::vector<OverlapResult> overlap;
				Physics::CircleOverlap(overlap, Vec2f(7, 0), 3, 1);
				if (overlap.size() > 0)
				{
					sprite.color = Vec4f(1, 0, 0, 1);
				}
				else
				{
					sprite.color = Vec4f(1, 1, 1, 1);
				}
			});
	}
	float timer = 0;
	float y = 0;
};
void RaycastTest()
{
	Engine::Init();
	Systems::CreateWorld();

	///__ b2 raycast
	LOG_INFO(" --- b2Raycast ---");
	b2World world(Vec2f(0, 0));

	b2BodyDef bodyDefault;
	auto body1 = world.CreateBody(&bodyDefault);

	b2PolygonShape shape;
	shape.SetAsBox(10, 10);
	b2FixtureDef fixtureDefault;
	fixtureDefault.shape = &shape;
	body1->CreateFixture(&fixtureDefault);

	RaycastCallbackClosest raycast1(65535);
	world.RayCast(&raycast1, Vec2f(100, 0), Vec2f(-100, 0));
	LOG_INFO("Raycast 1 hit? " + std::to_string(raycast1.result));

	body1->SetTransform(Vec2f(1000, 1000), 0);

	RaycastCallbackClosest raycast2(65535);
	world.RayCast(&raycast2, Vec2f(100, 0), Vec2f(-100, 0));
	LOG_INFO("Raycast 2 result? " + std::to_string(raycast2.result));

	b2CircleShape circle1;
	circle1.m_p = Vec2f(0, 0);
	circle1.m_radius = 4;

	b2CircleShape circle2;
	circle2.m_p = Vec2f(1, 0);
	circle2.m_radius = 4;

	b2Transform t;
	t.p = { 0, 0 };
	t.q = b2Rot(0);

	std::cout << "OVERLAP1: " << b2TestOverlap(&circle1, 0, &circle2, 0, t, t) << std::endl;
	circle2.m_p = { 1023, 0 };
	std::cout << "OVERLAP2: " << b2TestOverlap(&circle1, 0, &circle2, 0, t, t) << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	///_____ filter test

	LOG_INFO(" --- Filter ---");
	std::cout << std::endl;
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


	Filter fil;

	Bitmask m;
	for (int i = 0; i < 16; i++)
	{
		fil.AddFlag(std::to_string(i));
		m.AddFlag(fil.GetMask(std::to_string(i)).flags);
		std::cout << m.flags << std::endl;
	}



	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	///___ physics test
	LOG_INFO(" --- CustomPhysics ---");
	std::cout << std::endl;
		//circle overlap custom physics
	std::cout << " Circle overlap Sandbox::Body: " << std::endl;

	
	Physics::Instance();
	Systems::Push<CollisionTestSystem>();

	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);

	Entity bodyEntity = Entity::Create();

	auto body = bodyEntity.AddComponent<Body>(Body::Type::Kinematic, 1);
	auto trans = bodyEntity.AddComponent<Transform>();
	trans->SetRotationZAxis(12);
	auto spriteRender = bodyEntity.AddComponent<SpriteRender>();
	spriteRender->SetSprite(Assets::Get<Sprite>("spritesheet.png_0_0").Ptr());
	body->AddCollider(makesptr<Box2D>(7, 7));

	Entity circleEntity = Entity::Create();


	circleEntity.AddComponent<Transform>()->SetPosition(7, 0, 0);
	auto bo = circleEntity.AddComponent<Body>(Body::Type::Kinematic, 4);

	bo->AddCollider(makesptr<Circle2D>(3));

	Physics::DrawCollider(true);


	Engine::Launch();


}