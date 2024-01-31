#include "Sandbox/Render.h"
#include "Sandbox/Engine.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Assets.h"

using namespace Sandbox;
sptr<Sprite> LoadSpriteNeutral()
{
	auto texture = Assets::Get<Texture>("semi_transparent.png");
	return makesptr<Sprite>(texture.Ptr());
}

sptr<Sprite> LoadSpriteOrigin()
{
	auto texture = Assets::Get<Texture>("semi_transparent_green.png");
	auto sprite = makesptr<Sprite>(texture.Ptr());
	sprite->SetOrigin(Vec2f(0, 0.5));
	return sprite;
}

void CreateEntity(Vec3f pos, sptr<Sprite> sprite)
{
	Entity entity;
	entity.AddComponent<SpriteRender>()->SetSprite(sprite);
	entity.AddComponent<Transform>()->SetPosition(pos);
}

void BlendingTest()
{
	Engine::Init();

	Systems::CreateWorld();


	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &cam);
	auto sprite1 = LoadSpriteNeutral();
	auto sprite2 = LoadSpriteOrigin();

	CreateEntity(Vec3f(0), sprite1);
	CreateEntity(Vec3f(0, 0, -1), sprite2);

	Engine::Launch();
}

