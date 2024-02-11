#pragma once

#include <vector>
#include <earcut/mapbox/earcut.hpp>
#include <box2D/box2d.h>
#include "Sandbox/Vec.h"
#include "Sandbox/ECS/Entity.h"

namespace mapbox {
	namespace util {

		template <>
		struct nth<0, Sandbox::Vec3f> {
			inline static auto get(const Sandbox::Vec3f& t) {
				return t.x;
			};
		};
		template <>
		struct nth<1, Sandbox::Vec3f> {
			inline static auto get(const Sandbox::Vec3f& t) {
				return t.y;
			};
		};

		template <>
		struct nth<0, Sandbox::Vec2f> {
			inline static auto get(const Sandbox::Vec2f& t) {
				return t.x;
			};
		};
		template <>
		struct nth<1, Sandbox::Vec2f> {
			inline static auto get(const Sandbox::Vec2f& t) {
				return t.y;
			};
		};
	}
}

namespace Sandbox
{
	class Body;

	struct Triangle
	{
		Vec2f a;
		Vec2f b;
		Vec2f c;
	};

	struct FixtureUserData
	{
		Body* body;
		EntityId entity;
	};

	class Collider
	{
	public:
		virtual ~Collider(){}
		virtual bool ColliderOverlap(Collider* collider) = 0;
		virtual bool CircleOverlap(Vec2f point, float radius) = 0;
		virtual bool PointInside(Vec2f point) = 0;
		virtual FixtureUserData* GetFixtureUserData() = 0;
	private:

	};

	struct Box2D : public Collider
	{
		Box2D(Vec2f dimensions);
		Box2D(float width, float height);

		Vec2f dimensions;

		b2PolygonShape shape;
	};

	struct Circle2D : public Collider
	{
		Circle2D(float radius);
		
		float radius;
		b2CircleShape shape;
	};

	struct Polygon2D : public Collider
	{
	public:
		Polygon2D();
		Polygon2D(std::vector<Vec2f> points);
		void AddPoint(Vec2f point);
		void SetPoints(std::vector<Vec2f> points);
		void ClearPoints();
		void BakeTriangles();

		std::vector<Triangle>* GetTriangles();
		
	private:
		bool m_needBake;
		std::vector<Triangle> m_triangles;
		std::vector<std::vector<Vec2f>> m_points;
	};
}
