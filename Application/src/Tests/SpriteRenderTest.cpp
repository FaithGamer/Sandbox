#include "Sandbox/Engine.h"
#include "Sandbox/Render.h"
#include "Sandbox/ECS.h"

using namespace Sandbox;

void SpriteRenderTest()
{
	Engine::Init();

	uint32_t layer = Renderer2D::Instance()->AddLayer("Layer");
	//Renderer2D::Instance()->SetLayerScreenSpace(layer, std::vector<Vec2f>{ {-0.5, -0.5 }, { 0.5, -0.5 }, { 0.5, 0.5 }, { -0.5, 0.5 }});

	Entity camEnt = Entity::Create();
	auto camera =camEnt.AddComponent<Camera>();
	camera->SetAspectRatio(Window::GetAspectRatio());
	camera->SetOrthographic(false);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, camera);
	Systems::SetMainCamera(camera);

	TextureImportSettings settings;
	settings.wrapping = TextureWrapping::Clamp;
	settings.filtering = TextureFiltering::Nearest;
	sptr<Texture> texture = makesptr<Texture>("assets/textures/test_rect.png", settings);

	Rect texRect(0, 0, 1050, 1000);
	auto entity = Entity::Create();
	auto sp = makesptr<Sprite>(texture);
	auto sprite = entity.AddComponent<SpriteRender>();
	sprite->SetSprite(sp);

	sprite->SetLayer(layer);
	sprite->renderBatch = Renderer2D::Instance()->GetBatchId(sprite->GetLayer(), sprite->GetShader(), nullptr);

	auto transform = entity.AddComponent<Transform>();
	transform->SetPosition(Vec3f(0, 0, 0));
	transform->SetScale(0.1, 0.1, 1);

	Engine::Launch();
}