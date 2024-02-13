#include "pch.h"
#include "Sandbox/Physics/Collider.h"
#include "Sandbox/Vec.h"
#include "Sandbox/Physics/Body.h"

namespace Sandbox
{
	Box2D::Box2D()
	{
		m_shape.SetAsBox(1, 1);
		m_fixtureDef.shape = &m_shape;
	}

	Box2D::Box2D(Vec2f dimensions)
	{
		m_shape.SetAsBox(dimensions.x * 0.5f, dimensions.y * 0.5f);
		m_fixtureDef.shape = &m_shape;
	}

	Box2D::Box2D(float width, float height)
	{
		m_shape.SetAsBox(width * 0.5f, height * 0.5f);
		m_fixtureDef.shape = &m_shape;
	}

	void Box2D::SetBody(Body* body, b2Filter filter)
	{
		//Create fixture and attach to body
		b2Body* b2B = body->GetB2Body();
		auto fixture = b2B->CreateFixture(&m_fixtureDef);
		fixture->SetFilterData(filter);

	}

	bool Box2D::ColliderOverlap(Collider* collider)
	{
		return false;
	}

	bool Box2D::CircleOverlap(Vec2f point, float radius)
	{
		return false;
	}

	bool Box2D::PointInside(Vec2f point)
	{
		return false;
	}

	CollisionRender Box2D::GetCollisionRender()
	{
		CollisionRender render;
		render.line.AddPoint(m_shape.m_vertices[0]);
		render.line.AddPoint(m_shape.m_vertices[1]);
		render.line.AddPoint(m_shape.m_vertices[2]);
		render.line.AddPoint(m_shape.m_vertices[3]);
		render.line.AddPoint(m_shape.m_vertices[0]);
		render.line.SetColor(Vec4f(0, 1, 0, 1));
		render.line.SetWidth(0.3f);
		return render;
	}

	///
	/// 
	///Polygon
	///
	///


	Polygon2D::Polygon2D()
	{
		m_points.emplace_back(std::vector<Vec2f>());
	}
	Polygon2D::Polygon2D(std::vector<Vec2f> points)
	{
		m_points.emplace_back(points);
	}

	void Polygon2D::SetBody(Body* body, b2Filter filter)
	{
		BakeTriangles();
		if (m_triangles.size() < 3)
		{
			LOG_ERROR("Polygon2D collider have less than 3 vertices. Cannot add collider to body.");
			return;
		}
		int i = 0;

		b2FixtureDef def;
		def.filter = filter;
		std::vector<Vec2f> debugTriangles; //to do to delete
		
		while (i*3 < m_triangles.size())
		{
			b2Vec2* vertices = new b2Vec2[3];
			b2PolygonShape triangle;
			for (int j = 0; j < 3; j++)
			{
				vertices[j] = m_points[0][m_triangles[i * 3 + j]];
				debugTriangles.emplace_back(m_points[0][m_triangles[i * 3 + j]]);
			}
			triangle.Set(vertices, 3);

			
			def.shape = &triangle;
		
			b2Body* b2B = body->GetB2Body();
			auto fixture = b2B->CreateFixture(&def);

			delete[] vertices;
			i++;
		}
	}
	bool Polygon2D::ColliderOverlap(Collider* collider)
	{
		return false;
	}
	bool Polygon2D::CircleOverlap(Vec2f point, float radius)
	{
		return false;
	}
	bool Polygon2D::PointInside(Vec2f point)
	{
		return false;
	}
	CollisionRender Polygon2D::GetCollisionRender()
	{
		return CollisionRender();
	}
	void Polygon2D::AddPoint(Vec2f point)
	{
		m_points[0].emplace_back(point);
	}
	void Polygon2D::SetPoints(std::vector<Vec2f>& points)
	{
		m_points[0] = points;
	}
	void Polygon2D::BakeTriangles()
	{
		m_triangles = mapbox::earcut(m_points);
	}
}