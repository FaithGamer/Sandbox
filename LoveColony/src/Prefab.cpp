#include "pch.h"
#include "Prefab.h"
#include "Sandbox/ECS/Systems.h"
#include "Systems/ZisYSystem.h"
#include "Sandbox/Physics/Body.h"
#include "Components/Scent.h"

namespace Prefab
{
	Entity Wall(Vec2f position)
	{
		//Create entity
		Entity wall = Entity::Create();
		wall.AddComponent<Transform>()->SetPosition(position);

		//Create Body
		StaticBody* body = wall.AddComponent<StaticBody>(position, Physics::GetLayerMask("Walls"));
		sptr<Box2D> collider = makesptr<Box2D>(Vec2f(1, 1));
		body->AddCollider(collider);

		SpriteRender* render = wall.AddComponent<SpriteRender>();
		render->SetSprite(Assets::Get<Sprite>("Obstacles.png_0_2").Ptr()); //todo, optimize by having Prefab as a class and holding on the assets it needs
		//render->SetLayer(Renderer2D::GetLayerId("Map"));
		wall.AddComponent<ZisY>();
		return wall;
	}

}