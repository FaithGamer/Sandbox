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
		m_shape.SetAsBox(dimensions.x*0.5f, dimensions.y*0.5f);
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
}