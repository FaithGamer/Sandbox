#include "pch.h"
#include "Sandbox/Sandbox.h"

using namespace Sandbox;
Entity entity;
class Sys : public System
{
public:
	void OnStart() override
	{
		Window::GetFocusSignal()->AddListener(&Sys::OnWindowFocus, this);
		Window::GetMinimizedSignal()->AddListener(&Sys::OnWindowMinimized, this);
		Window::SetRenderWhenMinimized(false);
	}
	void OnWindowFocus(bool focus)
	{
		if (focus)
		{
			LOG_INFO("Focus gained");
		}
		else
		{
			LOG_INFO("Focus Lost");
		}
	}
	void OnWindowMinimized(bool focus)
	{
		if (!focus)
		{
			LOG_INFO("maximized!");
		}
		else
		{
			LOG_INFO("minimized.");
		}
	}
	void OnUpdate(Time delta) override
	{
		t += (float)delta;
		if (t > 3.f)
		{
			ForeachComponents<Animator>([](Animator& animator)
				{
					if (animator.currentStateName == "basic")
					{
						animator.speed = 4.f;
					}
					else if (animator.currentStateName == "speed")
						animator.SetAnimation("basic");
				
				});
			t = 0;
		}
	}
	float t = 0;
};

void AnimTest()
{
	Engine::Init();
	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);

	entity = Entity::Create();
	entity.AddComponent<Transform>();
	auto sprite = entity.AddComponent<SpriteRender>();
	sprite->SetSprite(Assets::Get<Sprite>("spritesheet.png_0_0").Ptr());
	auto anim = entity.AddComponent<Animator>();

	anim->AddAnimation("basic", "anim_test.anim");
	anim->AddAnimation("speed", "anim_test2.anim");
	anim->SetAnimation("basic");

	Systems::Push<Sys>();

	Engine::Launch();
}