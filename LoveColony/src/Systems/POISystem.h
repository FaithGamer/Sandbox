#pragma once

#include "Sandbox/ECS/System.h"

using namespace Sandbox;

class POISystem : public System
{
public:
	POISystem();
	void OnAddShelter(ComponentSignal signal);
	void OnAddFood(ComponentSignal signal);
	void OnRemoveShelter(ComponentSignal signal);
	void OnRemoveFood(ComponentSignal signal);
private:
};
