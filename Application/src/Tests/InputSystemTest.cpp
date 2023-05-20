#include "pch.h"

#include "Sandbox/Engine.h"
#include "Sandbox/Render.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Control.h"


using namespace Sandbox;

struct PlayerControlled
{
	bool jump = false;
};

struct Body
{
	Vec2f velocity;
};

class Controller : public System
{
	void OnStart() override
	{
		Input* jump = Inputs::GetInputMap()->GetInput("Jump");
		jump->signal.AddListener(&Controller::OnJump, this);
	}

	void OnJump(InputSignal* signal)
	{
		if (signal->GetBool())
		{
			ForEachComponent<PlayerControlled, Body>(
				[](PlayerControlled& component, Body& body)
				{
					body.velocity.y += 10;
				});
		}
	}
};

class PhysicsSystem : public System
{
public:
	int GetUsedMethod() override
	{
		return System::Method::FixedUpdate;
	}
protected:
	void OnFixedUpdate(Time time) override
	{
		ForEachComponent(&PhysicsSystem::Gravity);
	}

	void Gravity(Body& body)
	{
		body.velocity.y -= 0.1f;
		std::cout << body.velocity.y << std::endl;
	}

};


void InitInputs()
{
	InputMap& inputs = Inputs::GetCurrentInputMap();
	ButtonInput* jump = inputs.CreateButtonInput("Jump");
	jump->BindKey(KeyScancode::Space);
}

void InputSystemTest()
{

	Engine::Init();
	Systems::CreateWorld();

	Inputs::CreateInputMap();

	InitInputs();
	Entity* player = Systems::GetMainWorld()->CreateEntity();
	player->AddComponent<PlayerControlled>();
	player->AddComponent<Body>();

	Systems::Push<Controller>();
	Systems::Push<PhysicsSystem>();


	Engine::Launch();
}