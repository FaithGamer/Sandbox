#include "pch.h"
#include "POI.h"
#include "Sandbox/Sandbox.h"

void POIInit::CreateEntity()
{
	Entity entity = Entity::Create();

	entity.AddComponent<Transform>()->SetPosition(position);
	sptr<Sprite> sprite;

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

	if(sprite != nullptr)
		entity.AddComponent<SpriteRender>()->SetSprite(sprite);

}
