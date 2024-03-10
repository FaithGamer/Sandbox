#pragma once
#include "Sandbox/Core/Serialization.h"
using namespace Sandbox;

struct ColonistSettings : public Serializable
{
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

	float maxSpeed;
};