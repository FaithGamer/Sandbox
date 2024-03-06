#pragma once

#include <box2d/box2d.h>
#include <box2d/b2_distance.h>
#include "Sandbox/Vec.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/Physics/Collider.h"
#include "Sandbox/Physics/Body.h"
#include "Sandbox/Physics/Bitmask.h"
namespace Sandbox
{
	struct RaycastResult
	{
		EntityId entityId = EntityId(0);
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
		EntityId entityId;
	};

	/// @brief For internal use
	class RaycastCallbackClosest : public b2RayCastCallback
	{
	public:
		RaycastCallbackClosest(Bitmask mask);
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

	public:
		RaycastResult result;

	private:
		Bitmask m_mask;
	};

	/// @brief For internal use
	class QueryB2ShapeOverlapAll : public b2QueryCallback
	{
	public:
		QueryB2ShapeOverlapAll(b2Shape* shape, Bitmask mask);
		bool ReportFixture(b2Fixture* fixture) override;

	public:
		std::vector<OverlapResult>* results;

	private:
		Bitmask m_mask;
		b2Shape* m_shape;
	};

	/// @brief For internal use
	class QueryBodyOverlapAll : public b2QueryCallback
	{
	public:
		QueryBodyOverlapAll(Body* body, Bitmask mask);
		bool ReportFixture(b2Fixture* fixture) override;

	public:
		std::vector<OverlapResult>* results;

	private:
		Body* m_body;
		Bitmask m_mask;
	};

}
