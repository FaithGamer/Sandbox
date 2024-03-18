#include "pch.h"
#include "LayerBlending.h"
#include "Sandbox/Sandbox.h"
using namespace Sandbox;

void LayerBlending()
{
	Engine::Init();

	Camera camera;
	camera.SetOrthographic(true);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &camera);
	Systems::SetMainCamera(&camera);
	Renderer2D::AddLayer("Layer1");
	Renderer2D::AddLayer("Layer2");

	//Entity1
	auto entity = Entity::Create();
	entity.AddComponent<Transform>()->SetPosition(0.3, 0, 1);
	auto sprite = entity.AddComponent<SpriteRender>();
	sprite->SetSprite(Assets::Get<Sprite>("green_layer.png_0_0").Ptr());
	sprite->SetLayer(Renderer2D::GetLayerId("Layer1"));

	//Entity2
	auto entity2 = Entity::Create();
	entity2.AddComponent<Transform>()->SetPosition(-0.3, 0, 0);
	auto sprite2 = entity2.AddComponent<SpriteRender>();
	sprite2->SetSprite(Assets::Get<Sprite>("red_layer.png_0_0").Ptr());
	sprite2->SetLayer(Renderer2D::GetLayerId("Layer2"));


	Engine::Launch();

}