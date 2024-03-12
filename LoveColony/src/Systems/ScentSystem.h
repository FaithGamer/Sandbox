#pragma once


#include "Sandbox/ECS/System.h"
#include "Components/Scent.h"
#include "Settings/ScentSettings.h"

using namespace Sandbox;

class ScentSystem : public System
{
public:
	void OnStart() override;
	void OnFixedUpdate(Time delta) override;
	void TryCreateTrackScent(Vec2f position, Scent::Type type);
public:
	void CreateTrackScent(Vec2f position, Scent::Type type);
	ScentSettings trackSettings;
private:
	Bitmask m_scentMask;

};