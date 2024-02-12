#include "pch.h"
#include "Sandbox/Physics/Physics.h"
#include "Sandbox/Physics/Raycast.h"

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
		QueryOverlapAll query(&shape, mask);
		query.results = &results;

		//Query the world
		ins->m_world->QueryAABB(&query, aabb);

		//Results are now stored 
	}

	b2World* Physics::GetB2World()
	{
		return Instance()->m_world;
	}
}	