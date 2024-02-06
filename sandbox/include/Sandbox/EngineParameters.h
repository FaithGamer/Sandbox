#pragma once
#include <string>
#include "Sandbox/Singleton.h"
#include "Sandbox/Vec.h"
#include "Sandbox/Time.h"
#include "Sandbox/Serialization.h"

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