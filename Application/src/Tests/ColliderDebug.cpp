#include "Sandbox\Sandbox.h"

using namespace Sandbox;

struct Tag
{
	int tag;
};
class Sys : public System
{
public:
	void Create(bool tag)
	{
		Vec2f pos(Random::Range(-5.f, 5.f), Random::Range(-5.f, 5.f));
		auto entity = Entity::Create();
		entity.AddComponent<Transform>()->SetPosition(pos);
		auto body = entity.AddComponent<StaticBody>(Vec2f(pos));
		body->AddCollider(makesptr <Box2D>(1.f));
		if (tag)
		{
			entity.AddComponent<Tag>();
		}
	}
	void OnStart() override
	{
		//Create an entity with body

		for (int i = 0; i < 10; i++)
		{
			Create(false);
		}
		Create(true);
		for (int i = 0; i < 10; i++)
		{
			Create(false);
		}


	}
	void OnFixedUpdate(Time delta) override
	{

			ForeachEntities<StaticBody, Tag>([](Entity entity, StaticBody& body, Tag& t)
				{
					entity.Destroy();
				});
		
	}

	float time = 0;
};

void ColliderDebug()
{
	Engine::Init();
	Camera camera;
	camera.SetOrthographic(true);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &camera);
	Systems::SetMainCamera(&camera);

	Physics::DrawColliders(true);

	Systems::Push<Sys>();

	Engine::Launch();
}