#pragma once
#include <thread>
#include <mutex>
#include "Sandbox/Core/Vec.h"
#include "Sandbox/ECS/Entity.h"
#include "Init.h"
#include "POI.h"


using namespace Sandbox;

enum ColonistState : int
{
	SearchingFood = 0,
	SearchingShelter,
	Sacrificing,
	Interacting
};

struct ColonistInit : public Init
{
	void CreateEntity() override;
	Vec2f position = 0;
};

struct ColonistPhysics
{
	//State
	ColonistState state = SearchingFood; // state write value is AI thread responsability only
	unsigned int foodCarried = 0;

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
	float interpolationTimer = 0;

	//Scent
	float distanceFromLastScentDrop = 0;
	Entity lastFollowedScent;
	Entity trackScent;
	Entity poiScent;
	sptr<std::mutex> scentMutex;

	//POI
	Entity lastInteractedPOI;
	Entity interactingPOI;
	POIType interactingPOIType;
	Entity lastEncounteredPOI;
	float interactionTimer = 0;
	float poiDistance;

	//Queued for deletion
	bool dead = false;
};

struct ColonistBrain
{
	float wanderTimer = 0;
	float nextWanderTime = 0;
	float wanderDirection = 0;

};
