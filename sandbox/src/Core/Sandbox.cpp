#include "pch.h"
#include "Sandbox.h"
#include "Systems.h"
#include "Window.h"
#include "Parameters.h"
#include "Inputs.h"

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
		Window::Get()->Load(parameters.appName, parameters.startupWindowResolution);
		//add imgui singleton loading

		//Main loop
		play = true;
		while (play)
		{
			Systems::Get()->Update();
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

