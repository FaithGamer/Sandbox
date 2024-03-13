#include "pch.h"
#include "ScentSettings.h"

ScentSettings::ScentSettings() :
	radius(0.2f),
	time(3.f),
	repulsive(false)
	
{
}
void ScentSettings::Deserialize(Serialized& config)
{
}
Serialized ScentSettings::Serialize()
{
	return Serialized();
}