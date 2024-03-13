#pragma once

#include "Sandbox/ECS/System.h"
#include "Components/POI.h"
#include "Components/Colonist.h"

using namespace Sandbox;

struct POIInteractionResult
{
	ColonistState colonistState = SearchingFood;
	bool proceeded = false;
};
class POISystem : public System
{
public:
	POISystem();
	void OnAddShelter(ComponentSignal signal);
	void OnAddFood(ComponentSignal signal);
	void OnRemoveShelter(ComponentSignal signal);
	void OnRemoveFood(ComponentSignal signal);

	POIInteractionResult InteractPOI(Entity& poi, ColonistPhysics& colonist);
private:
	//POI interaction
	POIInteractionResult InteractShelter(Shelter* shelter, ColonistPhysics& colonist);
	POIInteractionResult InteractFood(Food* food, ColonistPhysics& colonist);
	
	//Shelter
	void IncrementShelterFood(Shelter* shelter, int count);
	void SetShelterLevel(Shelter* shelter, unsigned int level);
	void ComputeShelterSpawnRate(Shelter* shelter);
};
