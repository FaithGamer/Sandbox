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

struct ColonistInit
{
	Vec2f position = 0;
};

struct ColonistPhysics
{
	//State
	ColonistState state = SearchingFood;

	//Movement
	float wanderDirection = 0;
	float currentAngle = 0;
	float speed = 0;
	float loveSpeedMod = 0;
	Vec2f velocity = 0;
	Vec2f offset = 0;

	//Interpolation
	Vec2f position = 0;
	Vec2f prevPosition = 0;
	Vec2f nextPosition = 0;
	float interpolationTime = 0;

	//Scent
	float distanceFromLastScentDrop = 0;
	EntityId lastFollowedScent = EntityId(0);
	bool scentDropper = true;

	//Queued for deletion
	bool dead = false;
};

struct ColonistBrain
{
	float wanderTimer = 0;
	float nextWanderTime = 0;
	float wanderDirection = 0;
};
