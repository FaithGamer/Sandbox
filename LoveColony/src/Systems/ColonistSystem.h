#pragma once

#include "Sandbox/Sandbox.h"
#include "Settings/ColonistSettings.h"
#include "Components/Colonist.h"
using namespace Sandbox;


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

private:
	inline void AIDirection();
	inline void MoveAndCollide(ColonistPhysics& physics,
		Transform& transform,
		const Bitmask wallMask,
		const float delta,
		const float margin,
		const float hitboxRadius);
private:
	WorkerThread m_aiThread;
	sptr<Task<void>> m_aiTask;
};