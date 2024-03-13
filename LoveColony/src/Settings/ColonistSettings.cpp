#include "pch.h"
#include "ColonistSettings.h"

ColonistSettings::ColonistSettings()
	: maxSpeed(4.f),
	acceleration(10.f),
	steeringSpeed(45.f),
	wanderPower(75.f),
	wanderTimeMin(0.2f),
	wanderTimeMax(2.f),
	sensorDistance(0.2f),
	sensorRadius(1.f),
	sensorAngle(200.f),
	scentDistance(0.5f),
	interactionTime(1.4f)
{

}
void ColonistSettings::Deserialize(Serialized& config)
{
	maxSpeed = config.GetFloat("maxSpeed");
}

Serialized ColonistSettings::Serialize()
{
	Serialized config;

	config["maxSpeed"] = maxSpeed;

	return config;
}