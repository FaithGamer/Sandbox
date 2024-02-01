#include "pch.h"

#include "AssetsTest.h"
#include "Sandbox/Render.h"
#include "Sandbox/Assets.h"
#include "Sandbox/Engine.h"
#include "Sandbox/Serialization.h"
#include "Sandbox/ECS.h"

using namespace Sandbox;

void CreateEntity(Vec3f pos, Asset<Sprite> sprite)
{
	Entity entity;
	entity.AddComponent<SpriteRender>()->SetSprite(sprite.Ptr());
	entity.AddComponent<Transform>()->SetPosition(pos);
}
void AssetsTest()
{
	Engine::Init();

    Asset<Serialized> config = Assets::Get<Serialized>("jsontest.config");

	auto intptr = makesptr<int>(2);
	auto ptr2 = intptr;

	std::cout << config.Ptr()->GetFloat("Float");


	Systems::CreateWorld();


	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &cam);

	auto sprite1 = Assets::Get<Sprite>("spritesheet.png_0_0");
	auto sprite2 = Assets::Get<Sprite>("spritesheet.png_0_1");
	auto sprite3 = Assets::Get<Sprite>("spritesheet.png_1_0");
	auto sprite4 = Assets::Get<Sprite>("spritesheet.png_1_1");

	CreateEntity(Vec3f(-14, 10, 0), sprite1);
	CreateEntity(Vec3f(10, 10, 0), sprite2);
	CreateEntity(Vec3f(-10, -10, 0), sprite3);
	CreateEntity(Vec3f(10, -10, 0), sprite4);

	Engine::Launch();
}