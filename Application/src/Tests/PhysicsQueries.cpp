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
#include "PhysicsQueries.h"

using namespace Sandbox;

struct FollowMouse
{
	int tag;
};
Filter collisionLayers;

class BodySystem :public System
{
public:
	void OnStart() override
	{

		collisionLayers.AddFlag("Layer1");
		collisionLayers.AddFlag("Layer2");

		//Create a circle that will follow the mouse
		Entity mouseCircle = Entity::Create();

		mouseCircle.AddComponent<FollowMouse>();
		mouseCircle.AddComponent<Transform>();
		Body* circleBody = mouseCircle.AddComponent<Body>(Body::Type::Kinematic, collisionLayers.GetMask("Layer1"));

		circleBody->AddCollider(makesptr<Circle2D>(2.f));

		//Create a box with a sprite
		Entity box = Entity::Create();

		box.AddComponent<Transform>();
		Body* boxBody = box.AddComponent<Body>(Body::Type::Kinematic, collisionLayers.GetMask("Layer2"));
		boxBody->SetLayerMask(collisionLayers.GetMask("Layer1"));
		boxBody->AddCollider(makesptr<Box2D>(Vec2f(4, 4)));
		SpriteRender* sprite = box.AddComponent<SpriteRender>();
		sprite->SetSprite(Assets::Get<Sprite>("spritesheet.png_0_0").Ptr());

	}
	void OnUpdate(Time delta) override
	{
		Vec2f mousePos = Systems::GetMainCamera()->ScreenToWorld(GetMousePosition(), Window::GetSize());

		//Follow mouse
		ForeachComponents<FollowMouse, Transform>([&mousePos](FollowMouse& mouse, Transform& transform)
			{
				transform.SetPosition(mousePos);
			});

		//Check if circle overlap box
		ForeachComponents<Body, SpriteRender>([&](Body& body, SpriteRender& sprite)
			{
				std::vector<OverlapResult> results;
				//Physics::BodyOverlap(results, &body, collisionLayers.GetMask("Layer1"));
				body.OverlappingBodies(results);
				if (results.size() > 0)
				{
					sprite.color = Vec4f(1, 0, 0, 1);
				}
				else
				{
					sprite.color = Vec4f(1, 1, 1, 1);
				}
			});
	}
};

class RaycastSystem :public System
{
public:
	void OnStart() override
	{

		collisionLayers.AddFlag("Layer1");
		collisionLayers.AddFlag("Layer2");

		//Create a line that extends from the middle of the screen to the mouse
		Entity lineMouse = Entity::Create();


		lineMouse.AddComponent<Transform>();
		WireRender* line = lineMouse.AddComponent<WireRender>(2);

		line->AddPoint(Vec3f(0, 0, 0));

		//Create a box with a sprite
		Entity box = Entity::Create();

		box.AddComponent<Transform>()->SetPosition(8, 5, 0);
		Body* boxBody = box.AddComponent<Body>(Body::Type::Kinematic, collisionLayers.GetMask("Layer2"));
		boxBody->SetLayerMask(collisionLayers.GetMask("Layer1"));
		boxBody->AddCollider(makesptr<Box2D>(Vec2f(4, 4)));
		SpriteRender* sprite = box.AddComponent<SpriteRender>();
		sprite->SetSprite(Assets::Get<Sprite>("spritesheet.png_0_0").Ptr());

	}
	void OnUpdate(Time delta) override
	{
		Vec2f mousePos = Systems::GetMainCamera()->ScreenToWorld(GetMousePosition(), Window::GetSize());

		//Follow mouse
		ForeachComponents<WireRender>([&mousePos](WireRender& wire)
			{
				
			});

		//Check if circle overlap box
		ForeachComponents<Body, SpriteRender>([&](Body& body, SpriteRender& sprite)
			{
				std::vector<OverlapResult> results;
				//Physics::BodyOverlap(results, &body, collisionLayers.GetMask("Layer1"));
				body.OverlappingBodies(results);
				if (results.size() > 0)
				{
					sprite.color = Vec4f(1, 0, 0, 1);
				}
				else
				{
					sprite.color = Vec4f(1, 1, 1, 1);
				}
			});
	}
};

void PhysicsQueries()
{
	Engine::Init();

	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &cam);

	Systems::Push<BodySystem>();
	Physics::DrawCollider(true);

	Engine::Launch();
}