#include "pch.h"
#include "Colonist.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Render.h"
#include "Sandbox/Core/Assets.h"
#include "Systems/ZisYSystem.h"

void ColonistInit::CreateEntity()
{
	Entity entity = Entity::Create();

	//Create entity
	Entity colonist = Entity::Create();
	auto transform = colonist.AddComponent<Transform>();
	transform->SetPosition(position);
	transform->SetScale(1.f);

	//Create render 
	SpriteRender* render = colonist.AddComponent<SpriteRender>();
	render->SetSprite(Assets::Get<Sprite>("Colonists.png_0_2").Ptr()); //todo, optimize by having Prefab as a class and holding on the assets it needs
	//render->SetLayer(Renderer2D::GetLayerId("Map"));
	//Colonist components
	colonist.AddComponent<ColonistBrain>();
	auto physics  = colonist.AddComponent<ColonistPhysics>();
	physics->scentMutex = makesptr<std::mutex>();

	//Sprite render ordering
	colonist.AddComponent<ZisY>();
}