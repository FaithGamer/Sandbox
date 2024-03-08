#include "pch.h"
#include "Sandbox/Physics/Physics.h"
#include "Sandbox/Physics/AABBQueries.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Physics/ColliderRenderDebugSystem.h"

namespace Sandbox
{
	Physics::Physics()
	{
		m_world = new b2World({ 0, -1 });

	}
	Physics::~Physics()
	{
		delete m_world;
	}
	void Physics::RaycastClosest(RaycastResult& result, Vec2f start, Vec2f end, Bitmask mask)
	{
		auto ins = Instance();
		QueryRaycastCallbackClosest query(start, mask, &result);
		ins->m_world->RayCast(&query, start, end);
	}
	void Physics::RaycastAll(std::vector<RaycastResult>& results, Vec2f start, Vec2f end, Bitmask mask)
	{
		auto ins = Instance();
		QueryRaycastCallbackAll query(start, mask, &results);
		ins->m_world->RayCast(&query, start, end);
	}
	void Physics::BodyOverlap(std::vector<OverlapResult>& results, Body* body, Bitmask mask)
	{
		//to do TEST
		auto ins = Instance();

		b2AABB aabb = body->GetAABB();

		QueryBodyOverlapAll query(body, mask, &results);

		ins->m_world->QueryAABB(&query, aabb);
	}
	void Physics::CircleOverlap(std::vector<OverlapResult>& results, Vec2f pos, float radius, Bitmask mask)
	{
		auto ins = Instance();
			
		// Make a box englobing the circle
		b2AABB aabb;
		aabb.lowerBound = pos - radius;
		aabb.upperBound = pos + radius;

		//Create the query
		b2CircleShape shape;
		shape.m_p = pos;
		shape.m_radius = radius;
		QueryB2ShapeOverlapAll query(&shape, mask, &results);

		//Query the world
		ins->m_world->QueryAABB(&query, aabb);

		//Results are now stored 
	}

	void Physics::PointInside(std::vector<OverlapResult>& results, Vec2f pos, Bitmask mask)
	{
		auto ins = Instance();

		b2AABB aabb;
		aabb.lowerBound = pos - 0.01f;
		aabb.upperBound = pos + 0.01f;

		QueryPointInsideAll query(pos, mask, &results);

		ins->m_world->QueryAABB(&query, aabb);

	}
	void Physics::DrawCollider(bool draw)
	{
		if (draw)
		{
			Systems::Push<ColliderRenderDebugSystem>();
		}
		else
		{
			Systems::Remove<ColliderRenderDebugSystem>();
		}
	}

	b2World* Physics::GetB2World()
	{
		return Instance()->m_world;
	}
}	