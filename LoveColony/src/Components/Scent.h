#pragma once

#include "Sandbox/Core/Vec.h"
#include "Sandbox/ECS/EntityId.h"
#include "Init.h"

using namespace Sandbox;

struct Scent
{
	enum class Type : int
	{
		Food = 0,
		Shelter,
		Sacrifice,
		Player
	};

	EntityId poi = EntityId(0);
	float poiDistance = 0;
	float timeRemaining = 0;
	Type type = Scent::Type::Food;
};

struct ScentInit : public Init
{
	virtual void CreateEntity() override;
	Scent::Type type = Scent::Type::Food;
	Vec2f position = 0;
	float poiDistance = 0;
	bool draw = false;
};