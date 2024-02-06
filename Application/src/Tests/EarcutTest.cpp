#include "Sandbox/Render.h"
#include "Sandbox/Engine.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Assets.h"
#include <earcut/mapbox/earcut.hpp>

using namespace Sandbox;

//current plan for collisions:
//generates triangles from vertices
//map triangles to efficient container
//
namespace mapbox {
	namespace util {

		template <>
		struct nth<0, Vec3f> {
			inline static auto get(const Vec3f& t) {
				return t.x;
			};
		};
		template <>
		struct nth<1, Vec3f> {
			inline static auto get(const Vec3f& t) {
				return t.y;
			};
		};

		template <>
		struct nth<0, Vec2f> {
			inline static auto get(const Vec2f& t) {
				return t.x;
			};
		};
		template <>
		struct nth<1, Vec2f> {
			inline static auto get(const Vec2f& t) {
				return t.y;
			};
		};
	}
}
void DrawTriangle(Vec3f* points)
{
	Entity li;
	auto line = li.AddComponent<LineRenderer>(4);
	li.AddComponent<Transform>();

	line->AddPoint(points[0]);
	line->AddPoint(points[1]);
	line->AddPoint(points[2]);
	line->AddPoint(points[0]);
}
void EarcutTest()
{
	Engine::Init();

	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);

	Systems::CreateWorld();

	using Point = std::array<float, 2>;
	std::vector<std::vector<Vec2f>> polygon;

	// Fill polygon structure with actual data. Any winding order works.
	// The first polyline defines the main polygon.
	polygon.push_back({ {25, 0}, {25, 25}, {0, 25}, {0, 0},  });
	// Following polylines define holes.
	polygon.push_back({ {5, 0 }, {7, 0}, {7, 15}, {10, 15}, {10, 0}, {14, 0}, {14, 17}, {10, 17}, {5, 17} });
	//polygon.push_back({ {15, 3 }, {19, 3}, {19, 7} });


	std::vector<uint32_t> r = mapbox::earcut(polygon);


	for (int j = 0; j < r.size() / 3; j++)
	{
		Vec3f points[3];

		for (int i = 0; i < 3; i++)
		{
			auto index = r[j * 3 + i];
			Vec3f pos;
			auto s = polygon[0].size();

			if (index >= s)
				pos = polygon[1][index - s];
			else
				pos = polygon[0][index];

			points[i] = pos;
		}

		DrawTriangle(points);
	}

	Engine::Launch();
}