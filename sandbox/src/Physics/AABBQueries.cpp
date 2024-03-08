#include "pch.h"
#include "Sandbox/Physics/AABBQueries.h"

namespace Sandbox
{
	//
	//
	// Raycast Closest
	//
	//
	RaycastCallbackClosest::RaycastCallbackClosest(Bitmask mask) : m_mask(mask)
	{

	}
	float RaycastCallbackClosest::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
	{
		//Fixture's body is not contained in the collision mask
		if (!m_mask.Contains(fixture->GetFilterData().categoryBits))
			return -1.0f;

		//UserData contains entity id of fixture's body's entity
		auto data = static_cast<Collider::UserData*>((void*)(fixture->GetUserData().pointer));

		//Store raycast data
		result.entityId = data->entityId;
		result.point = point;
		result.normal = normal;
		//to do compute distance
		result.hit = true;

		return fraction;
	}

	//
	//
	// B2Shape overlap
	//
	//

	QueryB2ShapeOverlapAll::QueryB2ShapeOverlapAll(b2Shape* shape, Bitmask mask, std::vector<OverlapResult>* Results)
		: m_mask(mask), m_shape(shape), results(Results)
	{
	}
	bool QueryB2ShapeOverlapAll::ReportFixture(b2Fixture* fixture)
	{
		//Layer mask
		if (!m_mask.Contains(fixture->GetFilterData().categoryBits))
			return true;

		b2Transform identity(b2Vec2(0, 0), b2Rot(0));
		if (!b2TestOverlap(m_shape, 0, fixture->GetShape(), 0, identity, fixture->GetBody()->GetTransform()))
			return true; //No overlap

		//User data in the fixture's body
		auto data = static_cast<Collider::UserData*>((void*)(fixture->GetUserData().pointer));

		results->emplace_back(OverlapResult(data->entityId));


		// Continue the query.
		return true;
	}

	//
	//
	// Body Overlap
	//
	//

	QueryBodyOverlapAll::QueryBodyOverlapAll(Body* body, Bitmask mask, std::vector<OverlapResult>* Results)
		: m_body(body), m_mask(mask), results(Results)
	{

	}

	bool QueryBodyOverlapAll::ReportFixture(b2Fixture* fixture)
	{
		//Layer mask
		if (!m_mask.Contains(fixture->GetFilterData().categoryBits))
			return true;

		auto colliders = m_body->GetColliders();
		bool collide = false;
		for (int i = 0; i < colliders->size(); i++)
		{
			auto trans = fixture->GetBody()->GetTransform();
			if ((*colliders)[i]->B2ShapeOverlap(fixture->GetShape(), trans))
			{
				collide = true;
			}
		}
		if (!collide)
			return true;
		//User data in the fixture's body
		auto data = static_cast<Collider::UserData*>((void*)(fixture->GetUserData().pointer));

		results->emplace_back(OverlapResult(data->entityId));


		// Continue the query.
		return true;
	}

	//
	//
	// Point Inside
	//
	//

	QueryPointInsideAll::QueryPointInsideAll(Vec2f point, Bitmask mask, std::vector<OverlapResult>* Results)
		: m_point(point), m_mask(mask), results(Results)
	{

	}
	bool QueryPointInsideAll::ReportFixture(b2Fixture* fixture)
	{
		//Layer mask
		if (!m_mask.Contains(fixture->GetFilterData().categoryBits))
			return true;
		//Is point inside fixture
		if (!fixture->TestPoint(m_point))
			return true;
		//User data in the fixture's body
		auto data = static_cast<Collider::UserData*>((void*)(fixture->GetUserData().pointer));

		results->emplace_back(OverlapResult(data->entityId));

		// Continue the query.
		return true;
	}
}