#include "pch.h"
#include "Sandbox/Physics/Body.h"
#include "Sandbox/Physics/Physics.h"
#include "Sandbox/Math.h"

namespace Sandbox
{
	Body::Body(Type type, Bitmask layer) : m_type(type), m_layer(layer), m_mask(65535), m_YisZ(false)
	{
		b2BodyDef def;

		switch (type)
		{
		case Type::Static:
			def.type = b2BodyType::b2_staticBody;
			break;
		case Type::Kinematic:
			def.type = b2BodyType::b2_kinematicBody;
			break;
		case Type::Dynamic:
			LOG_ERROR("Dynamic body not implemented");
			//def.type = b2BodyType::b2_dynamicBody;
			break;
		}

		m_body = Physics::GetB2World()->CreateBody(&def);
		m_body->SetTransform(Vec2f(0, 0), 0);
	}

	Body::~Body()
	{
		//Free the b2Body in the b2World
		m_body->GetWorld()->DestroyBody(m_body);
	}

	void Body::SetLayer(Bitmask layer)
	{
		m_layer = layer;
	}

	void Body::SetLayerMask(Bitmask mask)
	{
		m_mask = mask;
	}

	void Body::AddCollider(sptr<Collider> collider)
	{
		//Attach collider and body
		collider->SetBody(this, GetB2Filter(), userData);
		m_colliders.emplace_back(collider);
	}

	void Body::ClearCollider()
	{
		//to do
	}

	void Body::OverlappingBodies(std::vector<OverlapResult>& results)
	{
		Physics::BodyOverlap(results, this, m_mask);
	}

	bool Body::BodyOverlap(Body* body)
	{
		//to do
		return false;
	}

	bool Body::CircleOverlap(Vec2f point, float radius)
	{
		//to do
		return false;
	}

	bool Body::PointInside(Vec2f point)
	{
		//to do
		return false;
	}

	void Body::UpdateTransform(Vec3f position, float rotation)
	{
		//todo scale, needs to recreate the fixture

		b2Vec2 pos(position.x, position.y);
		if (m_YisZ)
		{
			pos.y = position.z;
		}
		//box2D rotation is counter-clockwise
		m_body->SetTransform(pos, Math::Radians(-rotation));
	}

	void Body::SetYisZ(bool yIsZ)
	{
		m_YisZ = yIsZ;
	}

	Bitmask Body::GetLayer() const
	{
		return m_layer;
	}

	Bitmask Body::GetLayerMask() const
	{
		return m_mask;
	}

	Body::Type Body::GetType() const
	{
		return m_type;
	}

	b2Body* Body::GetB2Body()
	{
		return m_body;
	}

	b2Filter Body::GetB2Filter()
	{
		b2Filter filter;
		filter.categoryBits = m_layer.flags;
		filter.groupIndex = 0;
		filter.maskBits = m_mask.flags;
		return filter;
	}

	const std::vector<sptr<Collider>>* Body::GetColliders()
	{
		return &m_colliders;
	}

	b2AABB Body::GetAABB()
	{
		b2AABB aabb = m_colliders[0]->GetAABB();
		for (int i = 1; i < m_colliders.size(); i++)
		{
			auto caabb = m_colliders[i]->GetAABB();
			if (caabb.lowerBound.x < aabb.lowerBound.x)
				aabb.lowerBound.x = caabb.lowerBound.x;
			if (caabb.lowerBound.y < aabb.lowerBound.y)
				aabb.lowerBound.y = caabb.lowerBound.y;
			if (caabb.upperBound.x > aabb.upperBound.x)
				aabb.upperBound.x = caabb.upperBound.x;
			if (caabb.upperBound.y > aabb.upperBound.y)
				aabb.upperBound.y = caabb.upperBound.y;
		}
		return aabb;
	}

}