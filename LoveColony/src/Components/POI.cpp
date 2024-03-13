#include "pch.h"

#include "Sandbox/Sandbox.h"
#include "POI.h"
#include "Scent.h"
#include "Systems/ZisYSystem.h"

void POIInit::CreateEntity()
{
	Entity entity = Entity::Create();

	entity.AddComponent<Transform>()->SetPosition(position);
	sptr<Sprite> sprite;
	entity.AddComponent<POI>()->type = type;

	switch (type)
	{
	case POIType::Food:
	{
		auto food = entity.AddComponent<Food>();
		food->startFoodCount = startCount;
		food->foodCount = startCount;
		sprite = Assets::Get<Sprite>("Food.png_0_0").Ptr();
	}
	break;
	case POIType::Shelter:
	{
		auto shelter = entity.AddComponent<Shelter>();
		sprite = Assets::Get<Sprite>("Shelter.png_0_0").Ptr();
	}
	break;
	}

	auto body = entity.AddComponent<StaticBody>(position);
	body->SetLayer(Physics::GetLayerMask("POI"));
	body->AddCollider(makesptr<Circle2D>(hitboxRadius));

	if (sprite != nullptr)
		entity.AddComponent<SpriteRender>()->SetSprite(sprite);

	entity.AddComponent<ZisY>();

	//Add scent as child:

	Entity scentt = Entity::Create();
	float scentRadius = 3.f;

	auto transform = scentt.AddComponent<Transform>();
	transform->SetPosition(position.x, position.y, 100);
	transform->SetScale(scentRadius);

	Scent* scent = scentt.AddComponent<Scent>();
	scent->timeRemaining = 9999.f;
	scent->poiDistance = 0.f;
	scent->type = type;
	scent->poi = true;
	scent->position = position;

	auto scentBody = scentt.AddComponent<StaticBody>(position, Physics::GetLayerMask("Scent"));
	scentBody->AddCollider(makesptr<Circle2D>(scentRadius));

	entity.AddChild(scentt);

}

