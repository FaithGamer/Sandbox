#include "pch.h"
#include "Prefab.h"
#include "Sandbox/ECS/Systems.h"
#include "Systems/ColonistSystem.h"
#include "Sandbox/PhysicsEngine.h"
#include "Components/Colonist.h"

namespace Prefab
{
	Entity Colonist()
	{
		//Create entity
		Entity colonist = Entity::Create();
		colonist.AddComponent<Transform>();

		//Create Body
		/*Body* body = colonist.AddComponent<Body>(Body::Type::Kinematic, Physics::GetLayerMask("Colonist"));
		sptr<Circle2D> collider = makesptr<Circle2D>(0.2f);
		body->AddCollider(collider);*/

		//Create render on a child entity, so we can animate it's transform without altering hitbox
		//Entity colonistSprite = Entity::Create();
		//colonist.AddComponent<Transform>();
		SpriteRender* render = colonist.AddComponent<SpriteRender>();
		render->SetSprite(Assets::Get<Sprite>("Colonists.png_0_2").Ptr()); //todo, optimize by having Prefab as a class and holding on the assets it needs

		//Add the child
		//colonist.AddChild(colonistSprite);

		//Colonist components
		colonist.AddComponent<ColonistBrain>();
		colonist.AddComponent<ColonistPhysics>();

		return colonist;
	}
}