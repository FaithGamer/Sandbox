#pragma once

#include "Init.h"
#include "Sandbox/Core/Vec.h"

using namespace Sandbox;
enum class POIType
{
	Shelter,
	Food
};

class POIInit : public Init
{
public:
	void CreateEntity() override;
public:
	unsigned int startCount;
	Vec2f position;
	POIType type;
	float hitboxRadius;

};

struct POI
{
	POIType type;
};

struct Shelter
{
	unsigned int foodRequired = 1;
	unsigned int foodCount = 0;
	float spawnRate = 0;
	unsigned int level = 0;
};

struct Food
{
	unsigned int foodCount;
	unsigned int startFoodCount;
};
