#pragma once

#include <box2d/box2d.h>
#include "Sandbox/Vec.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/Physics/Collider.h"

namespace Sandbox
{
	struct RaycastResult
	{
		Entity entity;
		Vec2f point = 0;
		Vec2f normal = 0;
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
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
		{
			//fixture->GetUserData().pointer;
			auto data = static_cast<FixtureUserData*>((void*)(fixture->GetUserData().pointer));

			result.entity = data->entity;
			result.point = point;
			result.normal = normal;
			result.hit = true;

			return fraction;
		}
		RaycastResult result;
	};

	/// @brief For internal use
	class QueryCircleOverlapAny : public b2QueryCallback
	{
	public:
		QueryCircleOverlapAny(Vec2f pos, float radius)
		{

		}

		bool ReportFixture(b2Fixture* fixture) override
		{
			auto data = static_cast<FixtureUserData*>((void*)(fixture->GetUserData().pointer));
			


			// Continue the query.
			return true;
		}

		OverlapResult result;
	};

}
