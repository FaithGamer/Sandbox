#pragma once

#include "Sandbox/Core/Vec.h"
#include "Sandbox/ECS/EntityId.h"
#include "Init.h"
#include "POI.h"

using namespace Sandbox;

struct Scent
{
	/*enum class Type : int
	{
		Food = 0,
		Shelter,
		Sacrifice,
		Player
	};*/

	bool poi = false;
	float poiDistance = 0;
	float timeRemaining = 0;
	POIType type = POIType::Food;
	Vec2f position = 0;
};

struct ScentInit : public Init
{
	virtual void CreateEntity() override;
	POIType type = POIType::Food;
	Vec2f position = 0;
	float poiDistance = 0;
	bool draw = false;
	bool poi = false;
};