#include "pch.h"
#include "ScentSystem.h"
#include "Sandbox/Physics/Physics.h"
#include "Sandbox/Render.h"
#include "Sandbox/Core/Assets.h"

void ScentSystem::OnStart()
{
	m_scentMask = Physics::GetLayerMask("Scent");
}
void ScentSystem::OnFixedUpdate(Time delta)
{
	ForeachEntities<Scent>([&](Entity& entity, Scent& scent)
		{
			//Scent decay over time
			scent.timeRemaining -= (float)delta;
			if (scent.timeRemaining <= 0)
			{
				entity.Destroy();
				return;
			}

			//Debug color
			if (!m_showScent)
				return;

			float opacity = scent.timeRemaining / trackSettings.time;
			entity.GetComponent<SpriteRender>()->color.a = opacity;
		});
}

void ScentSystem::TryCreateTrackScent(const ScentInit& init)
{
	//Check for overlapping scent
	std::vector<OverlapResult> results;
	Physics::CircleOverlap(results, init.position, trackSettings.radius, m_scentMask);

	if (!results.empty())
	{
	
		float closest = 99999.f;
		EntityId closestScent = EntityId(0);

		//Closest overlapping scent of same type
		for (int i = 0; i < results.size(); i++)
		{
			Entity scent = Entity(results[i].entityId);
			if (scent.GetComponent<Scent>()->type == init.type
				&& results[i].distance < closest)
			{
				closest = results[i].distance;
				closestScent = results[i].entityId;
			}
		}

		//Reset closest overlapping scent's timer
		Entity(closestScent).GetComponent<Scent>()->timeRemaining = trackSettings.time;
	}
	else
	{
		//Create a new scent
		CreateTrackScent(init);
	}
}
void ScentSystem::DebugShowScent(bool show)
{
	//Show/Hide scent
	if (!show && m_showScent)
	{
		//Remove sprite render component
		ForeachEntities<Scent, SpriteRender>([](Entity entity, Scent& scent, SpriteRender& sprite)
			{
				entity.RemoveComponent<SpriteRender>();
			});
	}
	else if (show && !m_showScent)
	{
		//Add sprite render component
		ForeachEntities<Scent>([&](Entity entity, Scent& scent)
			{
				AddScentRenderer(entity, scent.type);
			});
	}
	m_showScent = show;
}

void ScentSystem::CreateTrackScent(const ScentInit& init)
{
	Entity entity = Entity::Create();

	auto transform = entity.AddComponent<Transform>();
	transform->SetPosition(init.position.x, init.position.y, 100);
	transform->SetScale(trackSettings.radius * 2);
	Scent* scent = entity.AddComponent<Scent>();
	scent->timeRemaining = trackSettings.time;
	scent->poiDistance = init.poiDistance;
	scent->type = init.type;

	auto body = entity.AddComponent<StaticBody>(init.position, Physics::GetLayerMask("Scent"));
	body->AddCollider(makesptr<Circle2D>(trackSettings.radius));

	if (m_showScent)
	{
		AddScentRenderer(entity, scent->type);
	}
}

Vec4f ScentSystem::ScentDebugColor(Scent::Type type)
{
	switch (type)
	{
	case Scent::Type::Food:
		return Vec4f(0, 1, 0, 1);
	case Scent::Type::Shelter:
		return Vec4f(0, 1, 0, 1);
	}
	return Vec4f(1);
}

inline void ScentSystem::AddScentRenderer(Entity scent, Scent::Type type)
{
	auto render = scent.AddComponent<SpriteRender>();
	render->SetSprite(Assets::Get<Sprite>("Circle.png_0_0").Ptr());
	render->color = ScentDebugColor(type);
	//render->SetLayer(Renderer2D::GetLayerId("Terrain"));
}
