#include "pch.h"

#include "Sandbox/Sandbox.h"
#include "POI.h"
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

}
