#include "pch.h"
#include "Sandbox/ECS/System.h"
#include "Sandbox/ECS/Systems.h"

namespace Sandbox
{
	std::vector<World*>& System::GetWorlds()
	{
		return Systems::GetWorlds();
	}
}