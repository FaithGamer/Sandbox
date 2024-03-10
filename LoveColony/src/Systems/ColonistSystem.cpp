#include "pch.h"
#include "ColonistSystem.h"
#include "Components/Colonist.h"

ColonistSystem::ColonistSystem()
{
	ListenAddComponent<ColonistPhysics>(&ColonistSystem::OnAddColonistPhysics);
}

void ColonistSystem::OnStart()
{

}

void ColonistSystem::OnUpdate(Time delta)
{

}

int ColonistSystem::GetUsedMethod()
{
	return 0;
}

void ColonistSystem::OnAddColonistPhysics(ComponentSignal signal)
{
	//Initialize colonist physics
}

void ColonistSystem::OnAddColonistBrain(ComponentSignal signal)
{
	//Initialize colonist brain
}
