#pragma once

#include "Sandbox/Core/Vec.h"
#include "Sandbox/ECS/EntityId.h"

using namespace Sandbox;

enum ColonistState : int
{
	SearchingFood = 0,
	SearchingShelter,
	Sacrificing,
	Interacting
};

struct ColonistPhysics
{
	float wanderDirection = 0;
	float currentAngle = 0;
	float speed = 0;
	float loveSpeedMod = 0;
	float wanderTimer = 0;
	float nextWanderTime = 0;
	Vec2f velocity = 0;
	Vec2f offset = 0;

	//Interpolation
	Vec2f prevPosition = 0;
	Vec2f nextPosition = 0;
	float interpolationTime = 0;
};

struct ColonistBrain
{
	ColonistState state = SearchingFood;
	EntityId lastFollowedScent = EntityId(0);
};
