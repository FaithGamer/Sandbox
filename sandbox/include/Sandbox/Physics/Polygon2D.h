#pragma once

#include <vector>
#include "Sandbox/Vec.h"
#include "earcut/mapbox/earcut.hpp"

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

	class Polygon2D
	{
	public:
		std::vector<Triangle> triangles;
		void AddPoint(Vec2f point);
		void SetPoints(Vec2f points);
		void BakeTriangles();
	private:
		std::vector<std::vector<Vec2f>> m_points;
	};
}
