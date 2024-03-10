#pragma once

#include <box2d/box2d.h>
#include <box2d/b2_distance.h>
#include "Sandbox/Core/Vec.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/Physics/Collider.h"
#include "Sandbox/Physics/Body.h"
#include "Sandbox/Physics/Bitmask.h"
namespace Sandbox
{

	/// @brief For internal use
	class QueryRaycastCallbackClosest : public b2RayCastCallback
	{
	public:
		QueryRaycastCallbackClosest(Vec2f start, Bitmask mask, RaycastResult* result);
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

	public:
		RaycastResult* result;

	private:
		Vec2f m_start;
		Bitmask m_mask;
	};

	/// @brief For internal use
	class QueryRaycastCallbackAll : public b2RayCastCallback
	{
	public:
		QueryRaycastCallbackAll(Vec2f start, Bitmask mask, std::vector<RaycastResult>* Results);
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

	public:
		std::vector<RaycastResult>* results;

	private:
		Vec2f m_start;
		Bitmask m_mask;
	};

	/// @brief For internal use
	class QueryB2ShapeOverlapAll : public b2QueryCallback
	{
	public:
		QueryB2ShapeOverlapAll(b2Shape* shape, Bitmask mask, std::vector<OverlapResult>* Results);
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
		QueryBodyOverlapAll(Body* body, Bitmask mask, std::vector<OverlapResult>* Results);
		bool ReportFixture(b2Fixture* fixture) override;

	public:
		std::vector<OverlapResult>* results;

	private:
		Bitmask m_mask;
		Body* m_body;
	};

	/// @brief For internal use
	class QueryBodyRaycastAll : public b2RayCastCallback
	{
	public:
		QueryBodyRaycastAll(Body* body, Bitmask mask, std::vector<OverlapResult>* Results);
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

	public:
		std::vector<OverlapResult>* results;

	private:
		Bitmask m_mask;
		Body* m_body;
	};

	/// @brief For internal use
	class QueryPointInsideAll : public b2QueryCallback
	{
	public:
		QueryPointInsideAll(Vec2f point, Bitmask mask, std::vector<OverlapResult>* Results);
		bool ReportFixture(b2Fixture* fixture) override;

	public:
		std::vector<OverlapResult>* results;

	private:
		Vec2f m_point;
		Bitmask m_mask;
	};

}
