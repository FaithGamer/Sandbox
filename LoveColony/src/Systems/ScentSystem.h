#pragma once


#include "Sandbox/ECS/System.h"
#include "Components/Scent.h"
#include "Settings/ScentSettings.h"
#include "Sandbox/Physics/Bitmask.h"
#include "Sandbox/Physics/Body.h"

using namespace Sandbox;

class ScentSystem : public System
{
public:
	void OnStart() override;
	void OnFixedUpdate(Time delta) override;
	/// @brief Create scent track (the one that colonists drop) or reset timer of closest overlaping one
	void TryCreateTrackScent(const ScentInit& init, std::vector<OverlapResult>& results);
	void DebugShowScent(bool show);
private:
	inline void CreateTrackScent(const ScentInit& init);
	inline Vec4f ScentDebugColor(Scent::Type type);
	inline void AddScentRenderer(Entity scent, Scent::Type type);
public:
	ScentSettings trackSettings;
private:
	Bitmask m_scentMask;
	bool m_showScent = false;
	float timerCount;

};