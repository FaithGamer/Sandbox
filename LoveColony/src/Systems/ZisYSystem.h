#pragma once
#include "Sandbox/ECS/System.h"

using namespace Sandbox;

struct ZisY
{
	int tag;
};

class ZisYSystem : public System
{
public:
	void OnUpdate(Time delta) override;
	int GetUsedMethod() override;
};

