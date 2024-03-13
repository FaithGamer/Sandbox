#pragma once

#include "Sandbox/ECS/System.h"
#include "Components/POI.h"
#include "Components/Colonist.h"

using namespace Sandbox;

class POISystem : public System
{
public:
	POISystem();
	void OnAddShelter(ComponentSignal signal);
	void OnAddFood(ComponentSignal signal);
	void OnRemoveShelter(ComponentSignal signal);
	void OnRemoveFood(ComponentSignal signal);

	void InteractShelter(Shelter& shelter, ColonistBrain& brain, ColonistPhysics& physics);
	void InteractFood(Food& shelter, ColonistBrain& brain, ColonistPhysics& physics);
private:
};
