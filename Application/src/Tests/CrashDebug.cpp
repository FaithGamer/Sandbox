#include "pch.h"
#include "Sandbox/Sandbox.h"

#define SANDBOX_NO_WINDOW

using namespace Sandbox;

class Sysss : public System
{
public:
	void OnUpdate(Time delta) override
	{
		t += (float)delta;
#ifndef SANDBOX_NO_WINDOW
		ForeachComponents<Transform>([&](Transform& transform)
			{
				auto pos = transform.GetPosition();
				pos.x = Math::Sin(t*50)*5;
				transform.SetPosition(pos);
			});
#else
		if (t >= 0.5)
		{
			t = 0;
			LOG_INFO("You can close the crash test.");
		}
#endif
	}

	float t = 0;
};

void CrashDebug()
{
	Engine::Init();

#ifndef SANDBOX_NO_WINDOW
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


#else
#endif

	Systems::Push<Sysss>();
	Engine::Launch();
}