#include "pch.h"
#include "POISettings.h"

POISettings::POISettings()
	:
	exclusionRadius(13.f),
	shelterSpacing(2.5f),
	shelterInnerRadius(7.f),
	shelterOuterRadius(10.f),
	shelterSpawnRateBase(0.1f),
	shelterSpawnRatePower(1.35f),
	shelterFoodReqBase(5.f),
	shelterFoodReqPower(1.8f),
	shelterLevelMax(20),
	treeSpacing(5.f)

{
}

void POISettings::Deserialize(Serialized& config)
{
}

Serialized POISettings::Serialize()
{
	return Serialized();
}
