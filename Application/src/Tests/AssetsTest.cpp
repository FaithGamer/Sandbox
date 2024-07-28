#include "pch.h"

#include "AssetsTest.h"
#include "Sandbox/Render.h"
#include "Sandbox/Core/Assets.h"
#include "Sandbox/Engine.h"
#include "Sandbox/Core/Serialization.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Core/Math.h"
#include <imgui/imgui.h>

using namespace Sandbox;

struct RootTag
{
	int tag = 0;
};
struct Tag2
{
	int tag;
};
Entity CreateEntity(Vec3f pos, Asset<Sprite> sprite)
{
	Entity entity = Entity::Create();
	entity.AddComponent<SpriteRender>()->SetSprite(sprite.Ptr());
	entity.AddComponent<Transform>()->SetPosition(pos);

	return entity;
}
class TransformSystem : public System
{
public:
	void OnImGui() override
	{
		ImGui::ShowDemoWindow();
	}
	void OnUpdate(Time deltaTime) override
	{
		time += (float)deltaTime * 100;

		ForeachComponents<RootTag, Transform>([deltaTime, this](RootTag& root, Transform& tr) {
			tr.RotateZ((float)deltaTime * 50.f);
			float s = Math::Lerp(1, 3, (Math::Sin((float)time) + 1) / 2);
			float zpos = Math::Lerp(1, -1, (Math::Sin((float)time) + 1) / 2);
			tr.SetScale(Vec3f(s, s, s));
			tr.SetPosition(tr.GetPosition().x, tr.GetPosition().y, zpos);
			});

		if (time > 300.f)
		{
			ForeachEntities<RootTag>([](Entity entity, RootTag& tag)
				{
					auto children = entity.GetComponent<Children>();
					if (children != nullptr)
					{
						for (auto& child : children->children)
						{
							Entity(child).Unparent();
						}
					}
				});
		}

		if (time > 400.f)
		{
			ForeachEntities<Tag2>([](Entity entity, Tag2& tag)
				{
					entity.Destroy();
				});
		}

		/*ForEachEntity<Transform>([deltaTime, this](Entity& entity, Transform& tr) {
			if (entity.GetComponent<RootTag>() != nullptr)
				return;

			tr.RotateZ((float)deltaTime * 50.f);
			//float s = Math::Lerp(1, 3, (Math::Sin((float)time) + 1) / 2);
			//float zpos = Math::Lerp(-1, 1, (Math::Sin((float)time) + 1) / 2);
			//tr.SetScale(Vec3f(s, s, s));
			//tr.SetPosition(tr.GetPosition().x, tr.GetPosition().y, zpos);
			});*/
	}
	float time = 0.f;
};
void AssetsTest()
{
	Engine::Init();

	Systems::Push<TransformSystem>();

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

	auto parent = CreateEntity(Vec3f(-14, 10, 1), sprite1);
	parent.AddComponent<RootTag>();
	auto child = CreateEntity(Vec3f(10, 0, 0), sprite2);
	child.AddComponent<Tag2>();
	parent.AddChild(child);
	//child.GetComponent<Transform>()->SetParent(parent.GetId());
	auto childchild = CreateEntity(Vec3f(0, -10, 0), sprite3);
	child.AddChild(childchild);


	Engine::Launch();
}