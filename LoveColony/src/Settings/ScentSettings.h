#pragma once

#include "Sandbox/Core/Serialization.h"
#include "Sandbox/Core/Vec.h"

using namespace Sandbox;

struct ScentSettings : Serializable
{
	ScentSettings();
	void Deserialize(Serialized& config) override;
	Serialized Serialize() override;

	float radius;
	float time;
	bool repulsive;
	Vec4f debugColor;
};
