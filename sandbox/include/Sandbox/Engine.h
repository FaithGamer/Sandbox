#pragma once

#define SANDBOX_IMGUI

namespace Sandbox
{
	class Engine
	{
	public:
		static void Init(bool logging = true);
		static void Launch();
		static void Stop();
	private:
		static bool play;
	};
}
