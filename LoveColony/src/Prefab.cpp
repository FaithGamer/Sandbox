#include "pch.h"
#include "Prefab.h"

namespace Prefab
{
	Entity Colonist()
	{
		Entity colonist = Entity::Create();
		colonist.AddComponent<Transform>();
		Entity colonistSprite = Entity::Create();
		colonistSprite.AddComponent<Transform>();
		SpriteRender* render = colonistSprite.AddComponent<SpriteRender>();
		render->SetSprite(Assets::Get<Sprite>("Colonists.png_0_0").Ptr());

	}
}