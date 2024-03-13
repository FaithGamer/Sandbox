#pragma once
#include "Sandbox/Core/Serialization.h"
using namespace Sandbox;

struct ColonistSettings : public Serializable
{
	//Default constructor
	ColonistSettings();

	//Serialization
	void Deserialize(Serialized& config) override;
	Serialized Serialize() override;

	//Data

	//Movement
	float maxSpeed;
	float acceleration;
	float steeringSpeed;
	float wanderPower;
	float wanderTimeMin;
	float wanderTimeMax;

	//Scent
	float sensorDistance;
	float sensorRadius;
	float sensorAngle;
	float scentDistance;

	//Poi
	float interactionTime;

};