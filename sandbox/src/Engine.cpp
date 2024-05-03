#include "pch.h"

#include "Sandbox/Engine.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/EngineParameters.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/Internal/ImGuiLoader.h"
#include "Sandbox/ECS/InputSystem.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Render/AnimationSystem.h"
#include "Sandbox/Core/Assets.h"
#include "Sandbox/ECS/SpriteRenderSystem.h"
#include "Sandbox/ECS/LineRendererSystem.h"
#include "Sandbox/ECS/WireRenderSystem.h"
#include "Sandbox/ECS/ParticleSystem.h"
#include "Sandbox/Physics/PhysicsSystem.h"
#include "Sandbox/Physics/Physics.h"
#include "Sandbox/Audio/Audio.h"


namespace Sandbox
{
	bool Engine::play = true;

	void Engine::Init()
	{
		Log::Init();

		LOG_INFO("Engine start.");
		EngineParameters params;
		Serialized paramsJson("assets/config/application.config");

		if (paramsJson.HadLoadError())
		{
			LOG_WARN("Couldn't load engine parameters, creating one with default values.");

			Serialized parametersWriteOnDisk = params.Serialize();
			parametersWriteOnDisk.WriteOnDisk("assets/config/application.config");
		}
		else
		{
			params = EngineParameters(paramsJson);
		}

		LOG_INFO("Loading window...");
		Window::Instance()->Init(params.appName, params.startupWindowResolution);
		Window::SetFullScreen(params.fullscreen);
		LOG_INFO("Loading assets...");
		Assets::Instance()->Init();
		LOG_INFO("Loading renderer...");
		Renderer2D::Instance();
		Renderer2D::AddLayer("DebugLayer");
		LOG_INFO("Loading Physics...");
		Physics::Instance();
		LOG_INFO("Creating world...");
		Systems::Instance()->CreateWorld();
		Systems::SetFixedUpdateTime(params.fixedUpdateTimeStep);
		

#ifdef SANDBOX_IMGUI
		LoadImGui(Window::GetSDLWindow(), Window::GetSDL_GLContext());
#endif

		//Default systems
		Systems::Push<InputSystem>();
		Systems::Push<SpriteRenderSystem>();
		Systems::Push<LineRendererSystem>();
		Systems::Push<WireRenderSystem>();
		Systems::Push<ParticleSystem>();
		Systems::Push<PhysicsSystem>();
		Systems::Push<AnimationSystem>();
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
}

