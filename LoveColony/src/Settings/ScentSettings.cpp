#include "pch.h"
#include "ScentSettings.h"

ScentSettings::ScentSettings() :
	radius(0.2f),
	time(20.f),
	repulsive(false),
	debugColor({0, 0, 1, 1})
	
{
}
void ScentSettings::Deserialize(Serialized& config)
{
}
Serialized ScentSettings::Serialize()
{
}