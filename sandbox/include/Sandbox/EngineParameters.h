#pragma once
#include <string>
#include "Sandbox/Singleton.h"
#include "Sandbox/Vec.h"
#include "Sandbox/Time.h"


namespace Sandbox
{
	//To do: enable serialization/deserialization
	struct EngineParameters
	{
		std::string appName = "Sandbox Application";
		std::string assetFolder = "assets";
		Vec2u startupWindowResolution = Vec2u(1000, 500);
		bool enableImGui = true;
		bool imGuiLightTheme = false;
		Time fixedUpdateTimeStep = 0.02f;
		bool useEngineSystems = true;

	};
}