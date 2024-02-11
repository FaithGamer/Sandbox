#include "pch.h"
#include "Sandbox/Physics/Physics.h"

namespace Sandbox
{
	/*class QueryCircleOverlap: public b2QueryCallback
	{
	public:


		bool ReportFixture(b2Fixture* fixture) override
		{
			b2Body* body = fixture->GetBody();
			
			b2Distance()
				bool inside = fixture->TestPoint(m_point);
				if (inside)
				{
					m_fixture = fixture;

					// We are done, terminate the query.
					return false;
				}
			

			// Continue the query.
			return true;
		}

		b2Fixture* m_fixture;
	};*/

	void Physics::CircleOverlap(std::vector<Body*> result, Vec2f pos, float radius)
	{
		auto ins = Instance();
		//b2Transform
			
		// Make a small box englobing the circle
		b2AABB aabb;
		aabb.lowerBound = pos - radius;
		aabb.upperBound = pos + radius;

		//QueryCircleOverlap query;

		//ins->m_world.QueryAABB(&query, circlAABB);
	}
}