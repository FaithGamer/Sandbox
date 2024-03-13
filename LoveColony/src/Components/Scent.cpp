#include "pch.h"
#include "Scent.h"
#include "Sandbox/Sandbox.h"
#include "Systems/ScentSystem.h"

void ScentInit::CreateEntity()
{
	auto sc = Systems::Get<ScentSystem>();
	Entity entity = Entity::Create();

	auto transform = entity.AddComponent<Transform>();
	transform->SetPosition(position.x, position.y, 100);
	transform->SetScale(sc->trackSettings.radius * 2);
	Scent* scent = entity.AddComponent<Scent>();
	scent->timeRemaining = sc->trackSettings.time;
	scent->poiDistance = poiDistance;
	scent->type = type;

	auto body = entity.AddComponent<StaticBody>(position, Physics::GetLayerMask("Scent"));
	body->AddCollider(makesptr<Circle2D>(sc->trackSettings.radius));

	if (draw)
	{
		sc->AddScentRenderer(entity, type);
	}
}