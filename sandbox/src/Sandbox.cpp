#include "pch.h"
#include "Sandbox/Sandbox.h"
#include "Sandbox/System/Systems.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Parameters.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/ImGuiLoader.h"

namespace Sandbox
{
	bool play = false;

	void Launch()
	{
		Launch(EngineParameters());
	}

	void Launch(const EngineParameters& parameters)
	{
		//Initialization
		Log::Init();
		Window::Get()->Load(parameters.appName, parameters.startupWindowResolution);
		LoadImGui(Window::GetSDLWindow(), Window::GetSDL_GLContext());
	
		//Main loop
		play = true;
		while (play)
		{
			Window::Clear();
			Systems::Get()->Update();
			Window::Render();
		}

		//Deallocation
		Inputs::Kill();
		Systems::Kill();
		Window::Kill();
	}

	void Stop()
	{
		play = false;
	}
}

