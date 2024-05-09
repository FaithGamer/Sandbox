#include "pch.h"
#include "Sandbox/Sandbox.h"

using namespace Sandbox;

class Sysss : public System
{
public:
	void OnUpdate(Time delta) override
	{
		t += (float)delta;
		ForeachComponents<Transform>([&](Transform& transform)
			{
				auto pos = transform.GetPosition();
				pos.x = Math::Sin(t*50)*5;
				transform.SetPosition(pos);
			});

	}
	float t = 0;
};

void CrashDebug()
{
	Engine::Init();
	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);

	Entity entity;
	entity = Entity::Create();
	entity.AddComponent<Transform>();
	auto render = entity.AddComponent<SpriteRender>();

	TextureImportSettings settings;
	settings.pixelPerUnit = 5;
	auto texture = makesptr<Texture>("assets/textures/spritesheet.png", settings);
	auto sprite = makesptr<Sprite>(texture);
	render->SetSprite(sprite);

	Systems::Push<Sysss>();

	Engine::Launch();
}