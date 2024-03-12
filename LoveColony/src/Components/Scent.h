#pragma once

#include "Sandbox/ECS/EntityId.h"

struct Scent
{
	enum Type : int
	{
		Food = 0,
		Shelter,
		Sacrifice,
		Player
	};

	EntityId poi;
	float poiDistance;
	float timeRemaining;
	Type type;
};
