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

struct PlayerControl
{
	bool jump = false;
};

class Main : public sb::System
{
	void OnStart() override
	{
		sb::Entity& player = sb::Systems::GetGameWorld()->CreateEntity();
		player.AddComponent<PlayerControl>();
	}
};

class Controller : public sb::System
{
	void OnStart() override
	{
		//Initialize an input map

		sb::InputMap map("InputMap");
		auto jump = makesptr<sb::ButtonInput>("Jump");
		jump->BindKey(sb::KeyScancode::Space);
		map.AddInput(jump);
		sb::Inputs::AddInputMap(map);

		jump->inputSignal.AddListener(&Controller::OnJump, this);
	}

	void OnJump(sb::InputSignal signal)
	{
		std::cout << " oui" << std::endl;
		auto world = sb::Systems::GetGameWorld();
		ForEachComponent(&Controller::Jump, world);
	}

	void Jump(sb::Entity& entity, PlayerControl& component)
	{
		component.jump = true;
	}
};

class Physics : public sb::System
{
	void OnUpdate(sb::Time time) override
	{
		ForEachComponent(&Physics::Jump, sb::Systems::GetGameWorld());
	}
	void Jump(PlayerControl& control)
	{
		if (control.jump)
		{
			std::cout << "Jump" << std::endl;
			control.jump = false;
		}
	}
};

int main(int argc, char** argv)
{
	sb::Engine::Init();
	sb::Systems::CreateGameWorld();

	sb::Systems::Push<ImGuiSystemTest>();
	sb::Systems::Push<Controller>();
	sb::Systems::Push<Physics>();
	sb::Systems::Push<Main>();

	sb::Engine::Launch();

	return 0;
}