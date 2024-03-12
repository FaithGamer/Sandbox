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

	float maxSpeed;
	float acceleration;
	float steeringSpeed;
	float wanderPower;
	float wanderTimeMin;
	float wanderTimeMax;

};