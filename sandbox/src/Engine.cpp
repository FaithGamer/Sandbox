#include "pch.h"

#include "Sandbox/Engine.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Parameters.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/ImGuiLoader.h"
#include "Sandbox/ECS/InputSystem.h"



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
		//Initialization
		Log::Init();

		//To do, call instance of every singleton allocate memory first
		Window::Instance()->Load(parameters.appName, parameters.startupWindowResolution);
		LoadImGui(Window::GetSDLWindow(), Window::GetSDL_GLContext());

		if (parameters.useEngineSystems)
		{
			Systems::Push<InputSystem>();
		}
	}

	void Engine::Launch()
	{
		//Main loop
		play = true;
		while (play)
		{
			Window::Clear();
			Systems::Instance()->Update();
			Window::Render();
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

