#pragma once

#include "Sandbox/Sandbox.h"
#include "Settings/ColonistSettings.h"
#include "Components/Colonist.h"
#include "Components/Scent.h"

using namespace Sandbox;
class ScentSystem;

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

	void AIUpdate();
	/// @brief This must be the ONLY way to destroy a colonist
	/// expect crash with multithreading otherwise
	/// @param colonist 
	void DestroyColonist(Entity colonist);
	/// @brief This must be the ONLY way to create a colonist
	/// expect crash with multithreading otherwise
	/// @param init 
	void CreateColonist(const ColonistInit& init);


private:
	inline void SyncPoint();
	inline void Steering(ColonistPhysics& physics, ColonistBrain& brain);
	inline void MoveAndCollide(ColonistPhysics& physics,
		Transform& transform,
		const Bitmask wallMask,
		const float delta,
		const float margin,
		const float hitboxRadius);
	bool ScentMatch(ColonistState state, Scent::Type scentType) const;
	void InstanceColonist(const ColonistInit& init);
private:
	std::mutex m_syncMutex;
	std::vector<Entity> m_destroy;
	std::vector<ColonistInit> m_create;
	ScentSystem* m_scentSystem;
	WorkerThread m_aiThread;
	WorkerThread m_physicsThread;
	sptr<Task<void>> m_aiTask;
	sptr<Task<void>> m_physicsTask;
};