#pragma once

#include "Sandbox/Sandbox.h"
#include "Settings/ColonistSettings.h"
#include "Components/Colonist.h"
#include "Components/Scent.h"
#include "Components/POI.h"
#include "GameManager.h"

using namespace Sandbox;
class ScentSystem;
class POISystem;

class ColonistSystem : public System
{
public:
	ColonistSystem();
	void OnStart() override;
	void OnUpdate(Time delta) override;
	void OnFixedUpdate(Time delta) override;

	int GetUsedMethod() override;

	void OnAddColonistPhysics(ComponentSignal signal);
	void OnAddColonistBrain(ComponentSignal signal);

	ColonistSettings settings;

	void OnSyncThread(ThreadSyncSignal signal);
	void AIUpdate(float delta);
	void PhysicsUpdate(float delta);


private:
	inline void FollowScent(ColonistPhysics& physics, const std::vector<OverlapResult>& overlaps);
	inline void DropScent(ColonistPhysics& physics, const std::vector<OverlapResult>& overlaps);
	inline void Steering(ColonistPhysics& physics, ColonistBrain& brain, float delta);
	inline void MoveAndCollide(ColonistPhysics& physics,
		const Bitmask wallMask,
		const float delta,
		const float margin,
		const float hitboxRadius);
	inline bool POIMatch(ColonistState state, POIType poiType) const;
	

private:

	ScentSystem* m_scentSystem;
	POISystem* m_poiSystem;
	GameManager* m_gameManager;

	uint16_t m_poiLayer;
	uint16_t m_wallLayer;
};