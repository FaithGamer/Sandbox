#include "Sandbox/Render.h"
#include "Sandbox/Engine.h"
#include "Sandbox/ECS.h"

using namespace Sandbox;
sptr<Sprite> LoadSprite()
{
	auto texture = makesptr<Texture>("assets/textures/semi_transparent.png");
	return makesptr<Sprite>(texture);
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
	auto sprite = LoadSprite();

	for (int i = 4; i >= 0; i--)
	{
		Vec3f pos(-4.f + i * 10, i, i*0.001f);
		CreateEntity(pos, sprite);
	}

	Engine::Launch();
}

