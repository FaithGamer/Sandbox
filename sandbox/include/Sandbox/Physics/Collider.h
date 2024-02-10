#pragma once

#include <vector>
#include <earcut/mapbox/earcut.hpp>
#include <box2D/box2d.h>
#include "Sandbox/Vec.h"

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
	struct Triangle
	{
		Vec2f a;
		Vec2f b;
		Vec2f c;
	};

	struct Box2D
	{
		Box2D();
		Box2D(float width, float height);

		b2FixtureDef b2Fixture;
		b2PolygonShape b2Shape;
	};

	struct Circle2D
	{
		Circle2D()
		Circle2D(float radius);
	};

	struct Polygon2D
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
