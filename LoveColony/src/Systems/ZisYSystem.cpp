#include "pch.h"
#include "ZisYSystem.h"
#include "Sandbox/Render/Transform.h"

void ZisYSystem::OnUpdate(Time delta)
{
	ForeachComponents<Transform, ZisY>([](Transform& transform, ZisY& tag)
		{
			Vec3f pos = transform.GetPosition();
			transform.SetPosition(pos.x, pos.y, pos.y);
		});
}

int ZisYSystem::GetUsedMethod()
{
	return System::Method::Update;
}
