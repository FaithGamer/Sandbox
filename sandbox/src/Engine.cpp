#include "pch.h"

#include "Sandbox/Engine.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/EngineParameters.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/ImGuiLoader.h"
#include "Sandbox/ECS/InputSystem.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/ECS/SpriteRenderSystem.h"
#include <stb/stb_image.h>

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
		Window::Instance()->Init(parameters.appName, parameters.startupWindowResolution);
		Renderer2D::Instance();

		LoadImGui(Window::GetSDLWindow(), Window::GetSDL_GLContext());
		stbi_set_flip_vertically_on_load(true);
		if (parameters.useEngineSystems)
		{
			Systems::Push<InputSystem>();
			Systems::Push<SpriteRenderSystem>();
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

}

