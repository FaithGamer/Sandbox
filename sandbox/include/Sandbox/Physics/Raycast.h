#pragma once

#include <box2d/box2d.h>
#include <box2d/b2_distance.h>
#include "Sandbox/Vec.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/Physics/Collider.h"
#include "Sandbox/Physics/Bitmask.h"
namespace Sandbox
{
	struct RaycastResult
	{
		Entity entity;
		Vec2f point = 0;
		Vec2f normal = 0;
		float distance = 0;
		bool hit = false;

		operator bool()
		{
			return hit;
		}
	};

	struct OverlapResult
	{
		Entity entity;
	};

	/// @brief For internal use
	class RaycastCallbackClosest : public b2RayCastCallback
	{
	public:
		RaycastCallbackClosest(Bitmask mask) : m_mask(mask)
		{

		}
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
		{
			if (!m_mask.Contains(fixture->GetFilterData().categoryBits))
				return -1.0f;
			//fixture->GetUserData().pointer;
			auto data = static_cast<FixtureUserData*>((void*)(fixture->GetBody()->GetUserData().pointer));

			//to do user data
			//result.entity = data->entity;
			result.point = point;
			result.normal = normal;
			//to do compute distance
			result.hit = true;

			return fraction;
		}
		RaycastResult result;
		Bitmask m_mask;
	};

	/// @brief For internal use
	class QueryOverlapAll : public b2QueryCallback
	{
	public:
		QueryOverlapAll(b2Shape* shape, Bitmask mask) : m_mask(mask), m_shape(shape)
		{
		}

		bool ReportFixture(b2Fixture* fixture) override
		{
			//Layer mask
			if (!m_mask.Contains(fixture->GetFilterData().categoryBits))
				return -1.0f;

			b2Transform identity(b2Vec2(0, 0), b2Rot(0));
			if (!b2TestOverlap(m_shape, 0, fixture->GetShape(), 0, identity, fixture->GetBody()->GetTransform()))
				return -1.0f;

			//User data in the fixture's body
			auto data = static_cast<FixtureUserData*>((void*)(fixture->GetUserData().pointer));

			//to do user data 
			//results->emplace_back(OverlapResult(Entity(data->entity)));
			results->emplace_back(OverlapResult(Entity(EntityId(0))));


			// Continue the query.
			return true;
		}

		std::vector<OverlapResult>* results;
	private:
		Bitmask m_mask;
		b2Shape* m_shape;
	};

}
