#pragma once

#include "Sandbox/Core/Serialization.h"
#include "Sandbox/Core/Vec.h"

using namespace Sandbox;

struct POISettings : Serializable
{
	POISettings();
	void Deserialize(Serialized& config) override;
	Serialized Serialize() override;

	float exclusionRadius;
	float shelterSpacing;
	float shelterInnerRadius;
	float shelterOuterRadius;
	float shelterSpawnRateBase;
	float shelterSpawnRatePower;
	int shelterFoodReqBase;
	float shelterFoodReqPower;
	unsigned int shelterLevelMax;
	float treeSpacing;

};
#pragma once
