#include "pch.h"
#include "Prefab.h"
#include "Sandbox/ECS/Systems.h"
#include "Systems/ZisYSystem.h"
#include "Sandbox/Physics/Body.h"
#include "Components/Scent.h"

namespace Prefab
{
	Entity Wall()
	{
		//Create entity
		Entity wall = Entity::Create();
		wall.AddComponent<Transform>();

		//Create Body
		Body* body = wall.AddComponent<Body>(Body::Type::Static, Physics::GetLayerMask("Wall"));
		sptr<Box2D> collider = makesptr<Box2D>(Vec2f(1, 1));
		body->AddCollider(collider);

		SpriteRender* render = wall.AddComponent<SpriteRender>();
		render->SetSprite(Assets::Get<Sprite>("Obstacles.png_0_2").Ptr()); //todo, optimize by having Prefab as a class and holding on the assets it needs
		render->SetLayer(Renderer2D::GetLayerId("Terrain"));
		wall.AddComponent<ZisY>();
		return wall;
	}

}