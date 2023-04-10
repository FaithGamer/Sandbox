#include "pch.h"
#include "Sandbox/Engine.h"

#include "Tests/ImGuiSystemTest.h"
#include "Tests/ECSTest.h"
#include "entt/entt.hpp"
#include "Sandbox/Time.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/std_macros.h"

#include <random>
#include <iostream>

namespace sb = Sandbox;

struct PlayerControlled
{
	bool jump = false;
};

template <typename... Args, typename F>
void foo(F f, Args... args)
{
	f(args...);
}

void bar(int b)
{
	std::cout << b << std::endl;
}
class Driver
{
public:
	Driver()
	{
		foo([](int b, int c){std::cout << b << " " << c << std::endl; }, 23, 32);
	}
};

class Controller : public sb::System
{
	void OnStart() override
	{
		sb::Input* jump = sb::Inputs::GetInputMap()->GetInput("Jump");
		jump->signal.AddListener(&Controller::OnJump, this);
	}

	void OnJump(sb::InputSignal signal)
	{
		ForEachComponent<PlayerControlled>(
			[](PlayerControlled& component) 
			{
				component.jump = true; 
			});
	}
};

class Physics : public sb::System
{
	void OnUpdate(sb::Time time) override
	{
		ForEachComponent(&Physics::Jump);
	}
	void Jump(PlayerControlled& control)
	{
		if (control.jump)
		{
			std::cout << "Jump" << std::endl;
			control.jump = false;
		}
	}
};

void InitInputs()
{
	sb::InputMap& inputs = sb::Inputs::GetCurrentInputMap();
	sb::ButtonInput* jump = inputs.CreateButtonInput("Jump");
	jump->BindKey(sb::KeyScancode::Space);
}

int main(int argc, char** argv)
{
	Driver driver;
	sb::Engine::Init();
	sb::Systems::CreateGameWorld();
	
	sb::Inputs::CreateInputMap();

	InitInputs();
	sb::Entity& player = sb::Systems::GetMainGameWorld()->CreateEntity();
	player.AddComponent<PlayerControlled>();

	sb::Systems::Push<ImGuiSystemTest>();
	sb::Systems::Push<Controller>();
	sb::Systems::Push<Physics>();
	

	sb::Engine::Launch();

	return 0;
}