#pragma once
#include <string>
#include "Singleton.h"
#include "Vec.h"
#include "Time.h"


namespace Sandbox
{
	struct EngineParameters
	{
		std::string appName = "Sandbox Application";
		std::string assetFolder = "assets";
		Vec2u startupWindowResolution = Vec2u(500, 500);
		bool enableImGui = true;
		Time fixedUpdateTimeStep = 0.02f;
	};

	class Parameters : public Singleton<Parameters> // useless ?
	{
	public:

	private:
		friend void Launch(const EngineParameters& parameters);
		static void SetInitalParameters(const EngineParameters& parameters);
		EngineParameters m_initialParameters;
	};
}