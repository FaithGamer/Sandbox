#include "pch.h"

#include "InputSystemTest.h"
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
	Vec2f position;
	Vec2f direction;
};

class Controller : public System
{
	void OnStart() override
	{
		//Listening to the jump input with the OnJump method
		sptr<Input> jump = Inputs::GetInputMap("Player1")->GetInput("Jump");
		jump->signal.AddListener(&Controller::OnJump, this);

		//Listening to the move input with the OnMove method
		sptr<Input> move = Inputs::GetInputMap("Player2")->GetInput("Move");
		move->signal.AddListener(&Controller::OnMove, this);
	}

	void OnJump(InputSignal* signal)
	{
		if (signal->GetBool())
		{
			ForeachComponents<PlayerControlled, Body>(
				[](PlayerControlled& component, Body& body)
				{
					body.position.y += 10;
				});
		}
	}

	void OnMove(InputSignal* signal)
	{
		Vec2f dir = signal->GetVec2f();

		ForeachComponents<PlayerControlled, Body>(
			[dir](PlayerControlled& component, Body& body)
			{
				body.direction = dir;
			});
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
		ForeachEntities(&PhysicsSystem::Gravity);
	}

	void Gravity(Entity entity, Body& body)
	{

		float gravity = 1.f;
		body.position.y -= gravity;
		body.position += body.direction;

		std::cout << "posX: " << body.position.x << std::endl;
		std::cout << "posY: " << body.position.y << std::endl;
	}

};


void InitInputs()
{
	//Create an input map
	sptr<InputMap> inputsPlayer1 = Inputs::CreateInputMap("Player1");

	//Create a button input
	sptr<ButtonInput> jump = inputsPlayer1->CreateButtonInput("Jump");
	jump->AddKey(KeyScancode::Space);
	jump->AddControllerButton(ControllerButton::X);

	//Create another input map
	sptr<InputMap> inputsPlayer2 = Inputs::CreateInputMap("Player2");

	//Create a directional input
	sptr<DirectionalInput> move = inputsPlayer2->CreateDirectionalInput("Move");
	std::vector<DirectionalButton> buttons
	{
		DirectionalButton(KeyScancode::W, Vec2f(0, 1)),
		DirectionalButton(KeyScancode::A, Vec2f(-1, 0)),
		DirectionalButton(KeyScancode::S, Vec2f(0, -1)),
		DirectionalButton(KeyScancode::D, Vec2f(1, 0)),
	};
	move->AddButtons(buttons);
	move->AddStick(ControllerStick::Left);

	//Trying to create an already existing map
	sptr<InputMap> inputs3 = Inputs::CreateInputMap("Player1");
	if (!inputs3)
	{
		LOG_INFO("inputs3 == nullptr");
	}
}

void InputSystemTest()
{

	Engine::Init();
	Systems::CreateWorld();

	InitInputs();

	Entity player = Entity::Create();
	player.AddComponent<PlayerControlled>();
	player.AddComponent<Body>();

	Systems::Push<Controller>();
	Systems::Push<PhysicsSystem>();


	Engine::Launch();
}