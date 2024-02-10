#pragma once

#include <box2d/box2d.h>
#include "Sandbox/Vec.h"
#include "Sandbox/ECS/Entity.h"

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

	class RaycastCallbackClosest : public b2RayCastCallback
	{
	public:
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
		{
			//fixture->GetUserData().pointer;

			result.point = point;
			result.normal = normal;
			result.hit = true;

			return fraction;
		}
		RaycastResult result;
	};

}
