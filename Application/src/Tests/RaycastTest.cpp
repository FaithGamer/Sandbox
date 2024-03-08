#include "pch.h"
#include "RaycastTest.h"

#include "Sandbox/ECS.h"
#include "Sandbox/Render.h"
#include "box2d/box2d.h"
#include "Sandbox/Engine.h"
#include "Sandbox/PhysicsEngine.h"
#include "Sandbox/Assets.h"
#include "Sandbox/Math.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Input/Mouse.h"

using namespace Sandbox;


struct MouseTag
{
	int tag;
};

struct PixelTag
{
	int tag;
};

class CollisionTestSystem : public System
{
public:
	void OnUpdate(Time delta) override
	{
		timer += (float)delta*50;
		rotation = Math::Sin(timer) * 180;

		Vec3f pos = Systems::GetMainCamera()->ScreenToWorld(GetMousePosition(), Window::GetSize());

		//Destroy old pixel
		ForeachEntities<PixelTag>([](Entity entity, PixelTag& tag)
			{
				entity.Destroy();
			});

		//Set circle to mouse position
		ForeachComponents<MouseTag, Transform>([&pos](MouseTag& tag, Transform& trans)
			{
				trans.SetPosition(pos);
			});

		//Rotate square body and create pixel over the hitbox
		ForeachEntities<Body, SpriteRender>([&](Entity entity, Body& body, SpriteRender& sprite)
			{
				auto trans = entity.GetComponent<Transform>();
				trans->SetRotation(rotation);
				std::vector<OverlapResult> overlap;
				Physics::CircleOverlap(overlap, pos, 3, 1);

				if (overlap.size() > 0)
				{
					sprite.color = Vec4f(1, 0, 0, 1);
				}
				else
				{
					sprite.color = Vec4f(1, 1, 1, 1);
				}

				/*auto aabb = body.GetAABB();

				for (int i = aabb.lowerBound.y*10; i < aabb.upperBound.y*10; i++)
				{
					for (int j = aabb.lowerBound.x*10; j < aabb.upperBound.x*10; j++)
					{
						Vec2f position((float)j / 10, (float)i / 10);

						std::vector<OverlapResult> overlap;
						Physics::PointInside(overlap, position, 1);
						if (overlap.size() > 0)
						{
							CreatePixel(position);
						}
					}
				}*/
			});
	}

	void CreatePixel(Vec3f pos)
	{
		Entity pixel = Entity::Create();
		pixel.AddComponent<PixelTag>();
		auto transform = pixel.AddComponent<Transform>();
		transform->SetPosition(pos);
		transform->SetScale(0.1f);

		auto sprite = pixel.AddComponent<SpriteRender>();
		sprite->SetSprite(Assets::Get<Sprite>("square.png_0_0").Ptr());
	}
	float timer = 0;
	float rotation = 0;
};
void RaycastTest()
{
	Engine::Init();
	
	///__ b2 raycast
	LOG_INFO(" --- b2Raycast ---");
	/*b2World world(Vec2f(0, 0));

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
	std::cout << std::endl;*/

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
	
	
	//circle overlap a rectangle
	std::cout << " Circle overlap rectangle  " << std::endl;

	Physics::DrawCollider(true);
	Systems::Push<CollisionTestSystem>();

	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);

	Window::Instance()->ResizeSignal.AddListener(&Camera::SetAspectRatio, &cam);

	Entity bodyEntity = Entity::Create();

	auto body = bodyEntity.AddComponent<Body>(Body::Type::Kinematic, 1);
	body->AddCollider(makesptr<Box2D>(7, 7));
	auto trans = bodyEntity.AddComponent<Transform>();
	trans->SetRotationZAxis(50);
	auto sprite = bodyEntity.AddComponent<SpriteRender>();
	sprite->SetSprite(Assets::Get<Sprite>("spritesheet.png_0_0").Ptr());
	

	Entity circleEntity = Entity::Create();

	circleEntity.AddComponent<MouseTag>();
	circleEntity.AddComponent<Transform>();
	auto bo = circleEntity.AddComponent<Body>(Body::Type::Kinematic, 4);

	bo->AddCollider(makesptr<Circle2D>(3));


	Engine::Launch();


}