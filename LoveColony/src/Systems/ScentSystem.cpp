#include "pch.h"
#include "ScentSystem.h"
#include "Sandbox/Physics/Physics.h"

void ScentSystem::OnStart()
{
	m_scentMask = Physics::GetLayerMask("Scent");
}
void ScentSystem::OnFixedUpdate(Time delta)
{
	ForeachEntities<Scent>([&](Entity& entity, Scent& scent)
		{
			scent.timeRemaining -= (float)delta;
			if (scent.timeRemaining <= 0)
			{
				entity.Destroy();
			}
		});
}

void ScentSystem::TryCreateTrackScent(Vec2f position, Scent::Type type)
{
	std::vector<OverlapResult> results;
	Physics::CircleOverlap(results, position, trackSettings.radius, m_scentMask);

}
