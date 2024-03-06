#include "pch.h"

#include "Sandbox/Engine.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/EngineParameters.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/ImGuiLoader.h"
#include "Sandbox/ECS/InputSystem.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Assets.h"
#include "Sandbox/ECS/SpriteRenderSystem.h"
#include "Sandbox/ECS/LineRendererSystem.h"
#include "Sandbox/ECS/WireRenderSystem.h"
#include "Sandbox/ECS/ParticleSystem.h"
#include "Sandbox/Physics/PhysicsSystem.h"


namespace Sandbox
{
	bool Engine::play = true;

	void Engine::Init()
	{
		EngineParameters params;
		Engine::Init(params);
	}

	void Engine::Init(const EngineParameters& parameters)
	{

		Log::Init();

		Window::Instance()->Init(parameters.appName, parameters.startupWindowResolution);
		Assets::Instance();
		Renderer2D::Instance();
		Renderer2D::AddLayer("DebugLayer");

		LoadImGui(Window::GetSDLWindow(), Window::GetSDL_GLContext());
	
		if (parameters.useEngineSystems)
		{
			Systems::Push<InputSystem>();
			Systems::Push<SpriteRenderSystem>();
			Systems::Push<LineRendererSystem>();
			Systems::Push<WireRenderSystem>();
			Systems::Push<ParticleSystem>();
			Systems::Push<PhysicsSystem>();
		}
	}

	void Engine::Launch()
	{
		//Main loop
		play = true;
		while (play)
		{
			Systems::Instance()->Update();
		}

		//Deallocation
		Inputs::Kill();
		Systems::Kill();
		Window::Kill();
	}

	void Engine::Stop()
	{
		play = false;
	}

	EngineParameters::EngineParameters(Serialized settings)
	{
		Deserialize(settings);
	}

	void EngineParameters::Deserialize(Serialized& settings)
	{
		appName = settings.GetString("AppName");

		startupWindowResolution.x = settings.GetArray<int>("Resolution")[0];
		startupWindowResolution.y = settings.GetArray<int>("Resolution")[1];

		enableImGui = settings.GetBool("EnableImGui");
		imGuiLightTheme = settings.GetBool("ImGuiLightTheme");
		fixedUpdateTimeStep = settings.GetFloat("FixedTimeStep");
		useEngineSystems = settings.GetBool("UseEngineSystems");
	}

	Serialized EngineParameters::Serialize()
	{
		return Serialized();
	}

}

