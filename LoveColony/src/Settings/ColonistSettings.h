#pragma once
#include "Sandbox/Core/Serialization.h"
using namespace Sandbox;

struct ColonistSettings : public Serializable
{
	//Default constructor
	ColonistSettings()
		: maxSpeed(2.f), acceleration(10.f)
	{

	}

	//Serialization
	void Deserialize(Serialized& config) override
	{
		maxSpeed = config.GetFloat("maxSpeed");
	}
	Serialized Serialize() override
	{
		Serialized config;

		config["maxSpeed"] = maxSpeed;

		return config;
	}

	//Data

	float maxSpeed;
	float acceleration;
};