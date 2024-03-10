#pragma once

#include "Sandbox/Core/Vec.h"
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
	//Movement
	float wanderDirection = 0;
	float currentAngle = 0;
	float speed = 0;
	float loveSpeedMod = 0;
	float wanderTimer = 0;
	float nextWanderTime = 0;
	Vec2f velocity = 0;
	Vec2f offset = 0;

};

struct ColonistBrain
{
	ColonistState state = SearchingFood;
};
