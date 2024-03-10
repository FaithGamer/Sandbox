#pragma once

#include "Sandbox/Sandbox.h"
using namespace Sandbox;


class ColonistSystem : public System
{
public:
	ColonistSystem();
	void OnStart() override;
	void OnUpdate(Time delta) override;

	int GetUsedMethod() override;

	void OnAddColonistPhysics(ComponentSignal signal);
	void OnAddColonistBrain(ComponentSignal signal);

};