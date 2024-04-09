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
		EngineParameters();
		EngineParameters(Serialized settings);
		void Deserialize(Serialized& parameters) override;
		Serialized Serialize() override;

		std::string appName;
		Vec2u startupWindowResolution;
		bool enableImGui;
		Time fixedUpdateTimeStep;
		bool fullscreen;

	};
}