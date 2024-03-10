#pragma once
#include <string>
#include "Sandbox/Internal/Singleton.h"
#include "Sandbox/Core/Vec.h"
#include "Sandbox/Core/Time.h"
#include "Sandbox/Core/Serialization.h"

namespace Sandbox
{
	//To do: enable serialization/deserialization
	struct EngineParameters : public Serializable
	{
		EngineParameters() = default;
		EngineParameters(Serialized settings);
		void Deserialize(Serialized& parameters) override;
		Serialized Serialize() override;

		std::string appName = "Sandbox Application";
		Vec2u startupWindowResolution = Vec2u(500, 500);
		bool enableImGui = true;
		bool imGuiLightTheme = false;
		Time fixedUpdateTimeStep = 0.02f;
		bool useEngineSystems = true;

	};
}