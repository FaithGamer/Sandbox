#include "pch.h"
#include "Sandbox/Engine.h"


#include "entt/entt.hpp"
#include "Sandbox/Vec.h"
#include "Sandbox/Time.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/std_macros.h"

#include "Tests/ImGuiSystemTest.h"
#include "Tests/ECSTest.h"
#include "Tests/BatchRendererTest.h"
#include "Tests/UniformBlockTest.h"
#include "Tests/RendererTest.h"
#include "Tests/FrameBufferTest.h"

#include <random>
#include <iostream>

namespace sb = Sandbox;

struct PlayerControlled
{
	bool jump = false;
};

struct Body
{
	sb::Vec2f velocity;
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
		ForEachComponent<PlayerControlled, Body>(
			[](PlayerControlled& component, Body& body)
			{
				body.velocity.y += 10;
			});
	}
};

class PhysicsSystem : public sb::System
{
public:
	int GetUsedMethod() override
	{
		return System::Method::FixedUpdate;
	}
protected:
	void OnFixedUpdate(sb::Time time) override
	{
		ForEachComponent(&PhysicsSystem::Gravity);
	}

	void Gravity(Body& body)
	{
		body.velocity.y -= 0.1f;
		std::cout << body.velocity.y << std::endl;
	}

};

class RenderSystem 
{

};

void InitInputs()
{
	sb::InputMap& inputs = sb::Inputs::GetCurrentInputMap();
	sb::ButtonInput* jump = inputs.CreateButtonInput("Jump");
	jump->BindKey(sb::KeyScancode::Space);
}

#include "Sandbox/ECS/Entity.h"
#include "Sandbox/Log.h"

int main(int argc, char** argv)
{
	/*sb::Engine::Init();
	sb::Systems::CreateWorld();

	sb::Inputs::CreateInputMap();

	InitInputs();
	sb::Entity& player = sb::Systems::GetMainWorld()->CreateEntity();
	player.AddComponent<PlayerControlled>();
	player.AddComponent<Body>();

	sb::Systems::Push<ImGuiSystemTest>();
	sb::Systems::Push<Controller>();
	sb::Systems::Push<PhysicsSystem>();


	sb::Engine::Launch();*/
	//BatchRendererTest();
	//
	
	//RendererTest();
	FrameBufferTest();


	return 0;
}