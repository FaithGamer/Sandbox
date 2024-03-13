#include "pch.h"
#include "POISystem.h"
#include "Sandbox/Core/Log.h"

POISystem::POISystem()
{
}
void POISystem::OnAddShelter(ComponentSignal signal)
{
}
void POISystem::OnAddFood(ComponentSignal signal)
{
}
void POISystem::OnRemoveShelter(ComponentSignal signal)
{
    //Delete all associated scent
}
void POISystem::OnRemoveFood(ComponentSignal signal)
{
    //Delete all associated scent
}

POIInteractionResult POISystem::InteractPOI(Entity& poi, ColonistPhysics& colonist)
{
    //Check if POI is valid and can be interacted
    ColonistState defaultState = SearchingFood;
    if (!poi.Valid())
    {
        return POIInteractionResult(defaultState, false);
    }
    auto poiTag = poi.GetComponent<POI>();
    if (poiTag == nullptr)
    {
        return POIInteractionResult(defaultState, false);
    }

    //Interact with the right type of component
    switch (poiTag->type)
    {
    case POIType::Food:
        return InteractFood(poi.GetComponentNoCheck<Food>(), colonist);
    case POIType::Shelter:
        return InteractShelter(poi.GetComponentNoCheck<Shelter>(), colonist);
    }
}

POIInteractionResult POISystem::InteractShelter(Shelter* shelter, ColonistPhysics& colonist)
{
    if (colonist.foodCarried < 1)
    {
        return POIInteractionResult(ColonistState::SearchingFood, false);
    }
    else
    {
        IncrementShelterFood(shelter, colonist.foodCarried);
        colonist.foodCarried = 0;
        return POIInteractionResult(ColonistState::SearchingFood, true);
    }
}

POIInteractionResult POISystem::InteractFood(Food* food, ColonistPhysics& colonist)
{
    if (colonist.foodCarried > 0)
    {
        LOG_WARN("Colonist is interacting with food but is already carrying food!");
        return POIInteractionResult(ColonistState::SearchingShelter, false);
    }
    if (food->foodCount < 1)
    {
        return POIInteractionResult(ColonistState::SearchingFood, false);
    }
    else
    {
        colonist.foodCarried += 1;
        food->foodCount -= 1;
        return POIInteractionResult(ColonistState::SearchingShelter, true);
    }
}

void POISystem::IncrementShelterFood(Shelter* shelter, int count)
{
    shelter->foodCount += count;
    if (shelter->foodCount >= shelter->foodRequired)
    {
        shelter->foodCount -= shelter->foodRequired;
        SetShelterLevel(shelter, shelter->level + 1);
    }
}

void POISystem::SetShelterLevel(Shelter* shelter, unsigned int level)
{
    shelter->level = level;
    ComputeShelterSpawnRate(shelter);
}

void POISystem::ComputeShelterSpawnRate(Shelter* shelter)
{
    //to do
}
