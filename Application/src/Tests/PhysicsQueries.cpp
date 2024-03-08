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
		//Mouse position
		Vec2f endLine = Systems::GetMouseWorldPos();

		//Reset box color to white
		ForeachComponents<SpriteRender>([](SpriteRender& sprite)
			{
				sprite.color = Vec4f(1, 1, 1, 1);
			});

		//Check for collision on the raycast
		RaycastResult result;
		Physics::RaycastClosest(result, Vec2f(0, 0), endLine);

		//Turn box red if hit
		if (result.hit)
		{
			auto sprite = Entity(result.entityId).GetComponent<SpriteRender>();
			if (sprite != nullptr)
			{
				sprite->color = Vec4f(1, 0, 1, 1);
			}
			//Cut the line to the hiting point
			endLine = result.point;
		}

		//Visualize raycast line from center screen to mouse
		ForeachComponents<WireRender>([&endLine](WireRender& wire)
			{
				wire.SetPointPosition(1, endLine);
			});

	}
};

struct ParentTag
{
	int tag;
};

class RaycastAllSystem :public System
{
public:
	Entity CreateBox(Vec3f position)
	{
		Entity box = Entity::Create();

		box.AddComponent<Transform>()->SetPosition(position);
		Body* boxBody = box.AddComponent<Body>(Body::Type::Kinematic, collisionLayers.GetMask("Layer2"));
		boxBody->SetLayerMask(collisionLayers.GetMask("Layer1"));
		boxBody->AddCollider(makesptr<Box2D>(Vec2f(4, 4)));
		SpriteRender* sprite = box.AddComponent<SpriteRender>();
		sprite->SetSprite(Assets::Get<Sprite>("spritesheet.png_0_0").Ptr());

		return box;
	}
	void OnStart() override
	{
		time = 0;
		collisionLayers.AddFlag("Layer1");
		collisionLayers.AddFlag("Layer2");

		//Create a line that extends from the middle of the screen to the mouse
		Entity lineMouse = Entity::Create();


		lineMouse.AddComponent<Transform>();
		WireRender* line = lineMouse.AddComponent<WireRender>(2);

		line->AddPoint(Vec3f(0, 0, 0));
		line->AddPoint(Vec3f(0, 0, 0));

		//Create 2 boxes with a sprite

		Entity box1 = CreateBox(Vec2f(8, 9));
		Entity box2 = CreateBox(Vec2f(15, 18));

		Entity parent = Entity::Create();
		parent.AddComponent<ParentTag>();
		parent.AddComponent<Transform>();
		parent.AddChild(box1);
		parent.AddChild(box2);


	}
	void OnUpdate(Time delta) override
	{


		//Mouse position
		Vec2f endLine = Systems::GetMouseWorldPos();

		//Reset box color to white
		ForeachComponents<SpriteRender>([](SpriteRender& sprite)
			{
				sprite.color = Vec4f(1, 1, 1, 1);
			});

		//Check for collision on the raycast
		std::vector<RaycastResult> results;
		Physics::RaycastAll(results, Vec2f(0, 0), endLine);

		//Turn box red if hit
		for (int i = 0; i < results.size(); i++)
		{
			auto sprite = Entity(results[i].entityId).GetComponent<SpriteRender>();
			if (sprite != nullptr)
			{
				sprite->color = Vec4f(1, 0, 1, 1);
			}
		}

		//Visualize raycast line from center screen to mouse
		ForeachComponents<WireRender>([&endLine](WireRender& wire)
			{
				wire.SetPointPosition(1, endLine);
			});

		time += (float)delta;

		ForeachComponents<ParentTag, Transform>([&](ParentTag& tag, Transform& transform) {
			float rotation = time * 10.f;
			transform.SetRotationZ(rotation);
			});
	}

	float time;
};

void PhysicsQueries()
{
	Engine::Init();

	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &cam);

	//Systems::Push<BodySystem>();
	//Systems::Push<RaycastSystem>();
	Systems::Push<RaycastAllSystem>();
	Physics::DrawCollider(true);

	Engine::Launch();
}