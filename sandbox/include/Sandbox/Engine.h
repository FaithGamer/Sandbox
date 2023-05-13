#pragma once

#include "EngineParameters.h"

namespace Sandbox
{
	class Engine
	{
	public:
		static void Init();
		static void Init(const EngineParameters& parameters);
		static void Launch();
		static void Stop();
	private:
		static bool play;
	};
}
